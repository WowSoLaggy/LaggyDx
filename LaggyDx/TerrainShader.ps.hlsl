// Terrain pixel shader: blends four textures by a baked shore mask, world height,
// and surface slope.
//
//   sand  - shorelines (ocean shore, lake edges, river banks), via a baked mask
//   grass - low, flat land
//   rock  - higher ground (raised plateau tiers)
//   cliff - steep faces (overrides everything wherever the ground is steep)
//
// Sand is driven by a per-vertex "shore factor" baked into input.tex.x by the
// terrain generator (1 = beach, 0 = inland). Height alone can't tell a shore from
// inland flat land - both sit at the same world Y - so proximity to water is
// resolved on the CPU and interpolated here. Grass/rock still come from world
// height (input.worldPos.y) and cliff from the surface normal (slope). Each
// texture is tiled by world XZ, so the vertex UV is reused only for the mask.

Texture2D sandTexture : register(t0);
Texture2D grassTexture : register(t1);
Texture2D rockTexture : register(t2);
Texture2D cliffTexture : register(t3);
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
static const float RockTile = 20.0;
static const float CliffTile = 18.0;

// Shore mask (input.tex.x, baked by the generator): how strongly the sand layer
// shows. The crossfade keeps the beach edge soft.
static const float SandMaskStart = 0.15; // below this -> no sand
static const float SandMaskFull = 0.55;  // above this -> full sand

// Height bands (world Y) for the non-shore layers. Each pair is the center
// transition height and the half-width of the smooth crossfade around it.
static const float GrassTop = 11.0;   // grass -> rock around the first plateau tier
static const float GrassBlend = 4.0;

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
  float3 rock = rockTexture.Sample(SampleType, input.worldPos.xz / RockTile).rgb;
  float3 cliff = cliffTexture.Sample(SampleType, input.worldPos.xz / CliffTile).rgb;

  // Base land blend by world height: grass on low flats -> rock higher up.
  // smoothstep gives a flat-tangent crossfade so the seam reads soft.
  float h = input.worldPos.y;
  float grassToRock = smoothstep(GrassTop - GrassBlend, GrassTop + GrassBlend, h);
  float3 land = lerp(grass, rock, grassToRock);

  // Shore override: paint sand where the baked mask says so. This is independent
  // of height, so a beach laps onto flat land near water without turning distant
  // flats (at the same height) into sand.
  float sandAmount = smoothstep(SandMaskStart, SandMaskFull, input.tex.x);
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
