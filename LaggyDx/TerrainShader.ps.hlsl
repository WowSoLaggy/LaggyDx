// Terrain pixel shader: blends three textures by world height and surface slope.
//
//   sand  - anything below flat land level (the underwater shore slope)
//   grass - all other land (flats and raised plateau tiers alike)
//   cliff - steep faces (overrides everything wherever the ground is steep)
//
// Sand is driven straight from world height (worldPos.y): only the shore slopes
// dip below flat-land level (PlaneHeight), so height alone identifies them. This
// means sand stops at the waterline and does not lap onto the flat land above it.
// Cliff comes from the surface normal (slope). Each texture is tiled by world XZ.

Texture2D sandTexture : register(t0);
Texture2D grassTexture : register(t1);
Texture2D cliffTexture : register(t2);
Texture2D shadowMap : register(t3);
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
  float4 shadowPos : TEXCOORD3;
};


// 3x3 PCF shadow-map lookup: 1 = fully lit, 0 = fully shadowed
float getShadowFactor(float4 i_shadowPos)
{
  float3 p = i_shadowPos.xyz / i_shadowPos.w;
  float2 uv = float2(p.x * 0.5 + 0.5, p.y * -0.5 + 0.5);

  // Outside the light's ortho box -> fully lit
  float result = 1.0;

  if (all(saturate(uv) == uv) && p.z >= 0.0 && p.z <= 1.0)
  {
    float w, h;
    shadowMap.GetDimensions(w, h);
    const float2 texel = float2(1.0 / w, 1.0 / h);

    float sum = 0.0;
    [unroll] for (int y = -1; y <= 1; ++y)
      [unroll] for (int x = -1; x <= 1; ++x)
        sum += shadowMap.SampleCmpLevelZero(ShadowSampler, uv + float2(x, y) * texel, p.z);

    result = sum / 9.0;
  }

  return result;
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

// Sand by world height: flat land sits at Y = 5 (PlaneHeight), only shore slopes
// go below it. Full sand below SandHeightFull, fading out up to SandHeightStart.
static const float SandHeightStart = 4.9; // above this -> no sand
static const float SandHeightFull = 1.0;  // below this -> full sand

// Slope (0 = flat, 1 = vertical wall). Above CliffStart the cliff texture takes
// over; CliffBlend is the crossfade width below it.
static const float CliffStart = 0.55;
static const float CliffBlend = 0.20;

// Grid overlay: world-space lines every gridCellSize units (cell size set from C++).
static const float GridLineHalfWidth = 0.08; // world units from a line's center to its edge
static const float3 GridLineColor = float3(0.05, 0.05, 0.05);
static const float GridLineOpacity = 0.5;


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

  // Sample each layer at its own tiling.
  float3 sand = sandTexture.Sample(SampleType, input.worldPos.xz / SandTile).rgb;
  float3 grass = grassTexture.Sample(SampleType, input.worldPos.xz / GrassTile).rgb;
  float3 cliff = cliffTexture.Sample(SampleType, input.worldPos.xz / CliffTile).rgb;

  // Grass covers all land that isn't shore or cliff.
  float3 land = grass;

  // Shore override: paint sand on ground that dips below flat-land level. Only the
  // shore slopes go there, so distant flats (at PlaneHeight) stay grass.
  float sandAmount = 1.0 - smoothstep(SandHeightFull, SandHeightStart, input.worldPos.y);
  float3 byHeight = lerp(land, sand, sandAmount);

  // Slope override: steep ground reads as cliff regardless of layer underneath.
  float cliffAmount = smoothstep(CliffStart - CliffBlend, CliffStart + CliffBlend, slope);
  float3 albedo = lerp(byHeight, cliff, cliffAmount);

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
