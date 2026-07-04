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
SamplerState SampleType;


cbuffer LightingCBuffer
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
  float specularIntensity;
  float specularPower;
  float2 _reserved;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float3 worldPos : TEXCOORD2;
};


// ----- Tunables ------------------------------------------------------------
// Height thresholds are in world units, keyed to the terrain generator:
// sea level is Y = 0, flat land sits at Y = 5 (PlaneHeight), and plateaus step
// up by 5 per tier (PlateauStep). Adjust these to taste and rebuild.

// Per-texture tiling: world units covered by one texture repeat. Larger = the
// texture stretches over more ground (coarser); smaller = it repeats more often.
static const float SandTile = 12.0;
static const float GrassTile = 16.0;
static const float CliffTile = 18.0;

// Sand by world height: flat land sits at Y = 5 (PlaneHeight), only shore slopes
// go below it. Full sand below SandHeightFull, fading out up to SandHeightStart.
static const float SandHeightStart = 4.9; // above this -> no sand
static const float SandHeightFull = 1.0;  // below this -> full sand

// Slope (0 = flat, 1 = vertical wall). Above CliffStart the cliff texture takes
// over; CliffBlend is the crossfade width below it.
static const float CliffStart = 0.55;
static const float CliffBlend = 0.20;


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

  float lightAmount = saturate(dot(N, -lightDirection));
  lightAmount = max(lightAmount, ambientStrength);
  textureColor.rgb *= lightAmount;

  // SPECULAR

  float3 reflection = reflect(lightDirection, N);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb * specularIntensity);

  return textureColor;
}
