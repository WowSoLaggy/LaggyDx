// Road pixel shader. Samples a single texture by the mesh's baked UVs (so the
// asphalt follows the spline ribbon) and applies the same diffuse + specular
// lighting as the terrain. Crossroad patches reuse this shader with a different
// bound texture.

Texture2D roadTexture : register(t0);
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


float4 main(PixelInputType input) : SV_TARGET
{
  // Re-normalize the interpolated normal so lighting stays stable across the
  // triangle (see the terrain PS for the rationale).
  float3 N = normalize(input.normal.xyz);

  float4 textureColor = roadTexture.Sample(SampleType, input.tex);
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
