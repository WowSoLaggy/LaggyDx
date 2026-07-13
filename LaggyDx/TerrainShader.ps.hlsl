// Terrain pixel shader: blends four textures by world height, slope, and noise.
//
//   sand  - anything below flat land level (the underwater shore slope)
//   grass - all other land (flats and raised plateau tiers alike)
//   dirt  - noise-driven bare patches carved out of the grass
//   cliff - steep faces (overrides everything wherever the ground is steep)
//
// Sand is driven straight from world height (worldPos.y): only the shore slopes
// dip below flat-land level (PlaneHeight), so height alone identifies them. This
// means sand stops at the waterline and does not lap onto the flat land above it.
// Cliff comes from the surface normal (slope) and is sampled triplanar so steep
// faces don't smear. Sand, grass, and dirt are tiled by world XZ.
//
// Anti-repetition: grass mixes two tilings of the same texture (their repeat
// periods interfere, so no tile boundary survives), value noise blends dirt
// patches into the grass, and a low-frequency brightness modulation over all
// layers breaks the "wallpaper" uniformity at distance.

// Must match Dx::c_shadowCascadesCount
#define CascadesCount 3

Texture2D sandTexture : register(t0);
Texture2D grassTexture : register(t1);
Texture2D cliffTexture : register(t2);
Texture2D shadowMap0 : register(t3);
Texture2D dirtTexture : register(t4);
Texture2D shadowMap1 : register(t5);
Texture2D shadowMap2 : register(t6);
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
// Height thresholds are in world units, keyed to the terrain generator:
// sea level is Y = 0, flat land sits at Y = 5 (PlaneHeight), and plateaus step
// up by 5 per tier (PlateauStep). Adjust these to taste and rebuild.

// Per-texture tiling: world units covered by one texture repeat. Larger = the
// texture stretches over more ground (coarser); smaller = it repeats more often.
static const float SandTile = 8.0;
static const float GrassTile = 4.0;
static const float CliffTile = 8.0;

// Anti-tiling second scale: a coarser, rotated sample of the same texture mixed
// in so the two repeat periods interfere and features change orientation.
// Non-integer ratios to the detail tile keep the periods unaligned.
static const float GrassTileCoarse = 23.0;
static const float GrassCoarseMix = 0.45; // 0 = detail only, 1 = coarse only
static const float DirtTileCoarse = 41.0;
static const float DirtCoarseMix = 0.5;

// Dirt patches: value noise thresholded into ragged worn-ground blotches. The
// blend stays partial (DryStrength) so grass always shows through, and the dirt
// is tinted darker/greener to sit under the grass tone instead of popping out.
static const float DirtTile = 9.0;         // world units per dirt texture repeat
static const float3 DirtTint = float3(0.72, 0.76, 0.60);
static const float DryPatchScale = 65.0;   // world units per noise feature
static const float DryEdgeLo = 0.60;       // noise below this -> fully grass
static const float DryEdgeHi = 0.80;       // noise above this -> max dirt
static const float DryStrength = 0.55;     // max dirt blend even inside a patch
static const float DryBreakupScale = 5.0;  // world units per interior grass speckle
static const float DryBreakupAmount = 0.4; // 0 = solid patches, 1 = fully mottled

// Macro variation: low-frequency brightness modulation over every layer.
static const float MacroScale = 90.0;      // world units per light/dark region
static const float MacroVariation = 0.13;  // +/- relative brightness swing

// Slope (0 = flat, 1 = vertical wall). Above CliffStart the cliff texture takes
// over; CliffBlend is the crossfade width below it.
static const float CliffStart = 0.55;
static const float CliffBlend = 0.20;
static const float TriplanarSharpness = 4.0; // higher = tighter blend zone between projection planes

// Sand by world height: flat land sits at Y = 5 (PlaneHeight), only shore slopes
// go below it. Full sand below SandHeightFull, fading out up to SandHeightStart.
static const float SandHeightStart = 4.9; // above this -> no sand
static const float SandHeightFull = 1.0;  // below this -> full sand

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

// Three-octave value noise: ragged enough for patch boundaries
float fbm3(float2 i_p)
{
  return valueNoise(i_p) * 0.55 + valueNoise(i_p * 2.63 + 17.17) * 0.28 + valueNoise(i_p * 6.71 + 47.3) * 0.17;
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

// Grass albedo at a world XZ: two-scale mix with noise-driven dirt patches
float3 getGrassColor(float2 i_worldXz)
{
  const float3 grass = sampleTwoScale(grassTexture, i_worldXz, GrassTile, GrassTileCoarse, GrassCoarseMix);
  const float3 dirt = sampleTwoScale(dirtTexture, i_worldXz, DirtTile, DirtTileCoarse, DirtCoarseMix) * DirtTint;

  // Bare patches: crossfade to dirt inside ragged noise blotches, with a fine
  // speckle keeping some grass alive in the interiors so they don't read as solid.
  float dryMask = smoothstep(DryEdgeLo, DryEdgeHi, fbm3(i_worldXz / DryPatchScale)) * DryStrength;
  dryMask *= 1.0 - DryBreakupAmount * valueNoise(i_worldXz / DryBreakupScale);
  return lerp(grass, dirt, dryMask);
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
  // the triangle shortens it - re-normalize per pixel so lighting and the slope
  // term don't wobble toward triangle centers.
  float3 N = normalize(input.normal.xyz);

  // Slope: 0 on flat ground, 1 on a vertical face.
  float slope = 1.0 - saturate(dot(N, float3(0.0, 1.0, 0.0)));

  // Sample each layer: sand planar, grass two-scale with dry patches, cliff triplanar.
  float3 sand = sandTexture.Sample(SampleType, input.worldPos.xz / SandTile).rgb;
  float3 grass = getGrassColor(input.worldPos.xz);
  float3 cliff = sampleTriplanar(cliffTexture, input.worldPos, N, CliffTile);

  // Grass covers all land that isn't shore or cliff.
  float3 land = grass;

  // Shore override: paint sand on ground that dips below flat-land level. Only the
  // shore slopes go there, so distant flats (at PlaneHeight) stay grass.
  float sandAmount = 1.0 - smoothstep(SandHeightFull, SandHeightStart, input.worldPos.y);
  float3 byHeight = lerp(land, sand, sandAmount);

  // Slope override: steep ground reads as cliff regardless of layer underneath.
  float cliffAmount = smoothstep(CliffStart - CliffBlend, CliffStart + CliffBlend, slope);
  float3 albedo = lerp(byHeight, cliff, cliffAmount);

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
