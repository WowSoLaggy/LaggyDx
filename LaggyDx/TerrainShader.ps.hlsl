// Terrain pixel shader: a grass base plus a stack of generic externally-authored paint layers.
//
// Each paint layer is a texture crossfaded over the result by its own mask
// channel: layer i reads channel i%4 of RGBA mask texture i/4. Masks are baked
// CPU-side and cover the whole map (worldPos.xz * paintInvMapSize). What a
// layer represents (sand, cliff, dirt, ...) is unknown here - the app owns the
// meaning, the bake, and the per-layer sampling params in PaintCBuffer:
// tiling, optional two-scale anti-tiling mix, tint, planar-XZ vs triplanar
// projection. Layers blend in slot order, so later slots win on overlap.
//
// Kept in-shader: the grass base (two tilings of the same texture whose repeat
// periods interfere, so no tile boundary survives), a low-frequency brightness
// modulation over all layers that breaks the "wallpaper" uniformity at
// distance, and the build-mode grid overlay.

// Must match Dx::c_shadowCascadesCount
#define CascadesCount 3
// Must match Dx::c_maxTerrainPaintLayers
#define MaxPaintLayers 8
// One RGBA mask texture carries 4 layer channels - must match Dx::c_terrainPaintMasksCount
#define PaintMasksCount ((MaxPaintLayers + 3) / 4)

Texture2D grassTexture : register(t0);
Texture2D shadowMap0 : register(t1);
Texture2D shadowMap1 : register(t2);
Texture2D shadowMap2 : register(t3);
Texture2D paintTextures[MaxPaintLayers] : register(t4); // t4..t11
Texture2D paintMasks[PaintMasksCount] : register(t12);  // t12..t13
SamplerState SampleType;
SamplerComparisonState ShadowSampler : register(s1);


cbuffer LightingCBuffer : register(b0)
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
  float specularIntensity;
  float specularPower;
  float2 _reserved;
};

cbuffer GridCBuffer : register(b1)
{
  float gridCellSize; // world units per grid cell; 0 or less hides the grid
  float3 _gridReserved;
};

struct PaintLayer
{
  float4 params; // x = tile, y = coarse tile, z = coarse mix, w = triplanar flag
  float4 tint;   // rgb = albedo tint, a = enabled (0 mutes the layer)
};

cbuffer PaintCBuffer : register(b2)
{
  float2 paintInvMapSize; // 1 / map world size, maps worldPos.xz to paint-mask UVs
  float2 _paintReserved;
  PaintLayer paintLayers[MaxPaintLayers];
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float3 worldPos : TEXCOORD2;
  float4 shadowPos[CascadesCount] : TEXCOORD3;
};


// 3x3 PCF lookup in one cascade: 1 = fully lit, 0 = fully shadowed, -1 = pixel outside this cascade
float sampleCascade(Texture2D i_map, float4 i_shadowPos)
{
  float3 p = i_shadowPos.xyz / i_shadowPos.w;
  float2 uv = float2(p.x * 0.5 + 0.5, p.y * -0.5 + 0.5);

  float w, h;
  i_map.GetDimensions(w, h);
  const float2 texel = float2(1.0 / w, 1.0 / h);

  float result = -1.0;

  // Inset by the PCF radius so the kernel never reads past the cascade edge
  const float2 margin = texel * 2.0;
  if (all(uv >= margin) && all(uv <= 1.0 - margin) && p.z >= 0.0 && p.z <= 1.0)
  {
    float sum = 0.0;
    [unroll] for (int y = -1; y <= 1; ++y)
      [unroll] for (int x = -1; x <= 1; ++x)
        sum += i_map.SampleCmpLevelZero(ShadowSampler, uv + float2(x, y) * texel, p.z);

    result = sum / 9.0;
  }

  return result;
}

// Samples the first (sharpest) cascade containing the pixel: 1 = fully lit, 0 = fully shadowed
float getShadowFactor(float4 i_shadowPos[CascadesCount])
{
  float factor = sampleCascade(shadowMap0, i_shadowPos[0]);
  if (factor < 0.0)
    factor = sampleCascade(shadowMap1, i_shadowPos[1]);
  if (factor < 0.0)
    factor = sampleCascade(shadowMap2, i_shadowPos[2]);

  // Outside all cascades -> fully lit
  return factor < 0.0 ? 1.0 : factor;
}


// ----- Tunables ------------------------------------------------------------

// Grass tiling: world units covered by one texture repeat, plus a coarser,
// rotated sample of the same texture mixed in so the two repeat periods
// interfere and features change orientation. Non-integer ratio keeps the
// periods unaligned.
static const float GrassTile = 4.0;
static const float GrassTileCoarse = 23.0;
static const float GrassCoarseMix = 0.45; // 0 = detail only, 1 = coarse only

// Macro variation: low-frequency brightness modulation over every layer.
static const float MacroScale = 90.0;     // world units per light/dark region
static const float MacroVariation = 0.13; // +/- relative brightness swing

static const float TriplanarSharpness = 4.0; // higher = tighter blend zone between projection planes

// Grid overlay: world-space lines every gridCellSize units (cell size set from C++).
static const float GridLineHalfWidth = 0.08; // world units from a line's center to its edge
static const float3 GridLineColor = float3(0.05, 0.05, 0.05);
static const float GridLineOpacity = 0.5;


// Cheap 2D hash: white noise in [0, 1)
float hash21(float2 i_p)
{
  float2 p = frac(i_p * float2(123.34, 345.45));
  p += dot(p, p + 34.345);
  return frac(p.x * p.y);
}

// Bilinearly-smoothed value noise in [0, 1)
float valueNoise(float2 i_p)
{
  const float2 cell = floor(i_p);
  const float2 f = frac(i_p);
  const float2 u = f * f * (3.0 - 2.0 * f);

  const float a = hash21(cell);
  const float b = hash21(cell + float2(1.0, 0.0));
  const float c = hash21(cell + float2(0.0, 1.0));
  const float d = hash21(cell + float2(1.0, 1.0));

  return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y);
}

// Two-octave value noise: enough irregularity for slow brightness drift
float fbm(float2 i_p)
{
  return valueNoise(i_p) * 0.65 + valueNoise(i_p * 2.63 + 17.17) * 0.35;
}

// Triplanar sample: projects along all three axes and blends by the normal, so
// near-vertical faces get a proper projection instead of a stretched XZ one.
float3 sampleTriplanar(Texture2D i_texture, float3 i_worldPos, float3 i_normal, float i_tile)
{
  float3 w = pow(abs(i_normal), TriplanarSharpness);
  w /= w.x + w.y + w.z;

  const float3 alongX = i_texture.Sample(SampleType, i_worldPos.zy / i_tile).rgb;
  const float3 alongY = i_texture.Sample(SampleType, i_worldPos.xz / i_tile).rgb;
  const float3 alongZ = i_texture.Sample(SampleType, i_worldPos.xy / i_tile).rgb;

  return alongX * w.x + alongY * w.y + alongZ * w.z;
}

// Anti-tiling sample: mixes a detail tiling with a coarser rotated one so the
// repeat periods interfere and the features' orientation decorrelates too.
float3 sampleTwoScale(Texture2D i_texture, float2 i_worldXz, float i_tile, float i_tileCoarse, float i_coarseMix)
{
  const float3 detail = i_texture.Sample(SampleType, i_worldXz / i_tile).rgb;
  const float2x2 rot = float2x2(0.8253, -0.5646, 0.5646, 0.8253); // fixed ~34 deg
  const float3 coarse = i_texture.Sample(SampleType, mul(rot, i_worldXz) / i_tileCoarse).rgb;
  return lerp(detail, coarse, i_coarseMix);
}


// Anti-aliased grid-line mask at a world XZ: 1 on a line, 0 between lines.
float getGridFactor(float2 i_worldXz)
{
  // Clamp keeps the division NaN-free when the grid is off; the result is discarded then.
  const float cellSize = max(gridCellSize, 0.001);
  const float2 dist = abs(frac(i_worldXz / cellSize + 0.5) - 0.5) * cellSize;
  const float2 aa = fwidth(i_worldXz) + 0.0001; // ~one pixel in world units, for smooth distant lines
  const float2 lineMask = 1.0 - smoothstep(GridLineHalfWidth, GridLineHalfWidth + aa, dist);
  return max(lineMask.x, lineMask.y);
}


float4 main(PixelInputType input) : SV_TARGET
{
  // The VS normal is unit-length at each vertex, but linear interpolation across
  // the triangle shortens it - re-normalize per pixel so lighting and the
  // triplanar weights don't wobble toward triangle centers.
  float3 N = normalize(input.normal.xyz);

  // Grass base: two-scale mix so no tile boundary survives.
  float3 albedo = sampleTwoScale(grassTexture, input.worldPos.xz, GrassTile, GrassTileCoarse, GrassCoarseMix);

  // Fan the baked mask channels out to one weight per layer.
  const float2 maskUv = input.worldPos.xz * paintInvMapSize;
  float maskValues[MaxPaintLayers];
  [unroll]
  for (int m = 0; m < PaintMasksCount; ++m)
  {
    const float4 mask = paintMasks[m].Sample(SampleType, maskUv);
    maskValues[m * 4 + 0] = mask.r;
    maskValues[m * 4 + 1] = mask.g;
    maskValues[m * 4 + 2] = mask.b;
    maskValues[m * 4 + 3] = mask.a;
  }

  // Paint layers: crossfade each over the result by its mask, in slot order.
  // The loop must stay [unroll]: SM 5.0 only allows literal indices into texture arrays.
  [unroll]
  for (int i = 0; i < MaxPaintLayers; ++i)
  {
    const float4 params = paintLayers[i].params;
    const float3 planar = sampleTwoScale(paintTextures[i], input.worldPos.xz, params.x, params.y, params.z);
    const float3 projected = sampleTriplanar(paintTextures[i], input.worldPos, N, params.x);
    const float3 color = (params.w > 0.5 ? projected : planar) * paintLayers[i].tint.rgb;
    albedo = lerp(albedo, color, maskValues[i] * paintLayers[i].tint.a);
  }

  // Macro variation: slow brightness drift so distant ground isn't uniform.
  const float macro = fbm(input.worldPos.xz / MacroScale);
  albedo *= 1.0 + (macro - 0.5) * 2.0 * MacroVariation;

  float4 textureColor = float4(albedo, 1.0);
  textureColor *= diffuseColor;

  // DIFFUSE

  float shadow = getShadowFactor(input.shadowPos);
  float lightAmount = saturate(dot(N, -lightDirection)) * shadow;
  lightAmount = max(lightAmount, ambientStrength);
  textureColor.rgb *= lightAmount;

  // SPECULAR

  float3 reflection = reflect(lightDirection, N);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb * specularIntensity * shadow);

  // GRID (applied after lighting so the lines stay readable in shadow)

  const float gridAmount = getGridFactor(input.worldPos.xz) * (gridCellSize > 0.0 ? GridLineOpacity : 0.0);
  textureColor.rgb = lerp(textureColor.rgb, GridLineColor, gridAmount);

  return textureColor;
}
