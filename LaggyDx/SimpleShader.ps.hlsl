Texture2D shaderTexture;
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
};


float4 main(PixelInputType input) : SV_TARGET
{
  // The VS normal is unit-length at each vertex, but linear interpolation
  // across the triangle shortens it — re-normalize per pixel so diffuse and
  // (especially) specular don't darken/wobble toward triangle centers.
  float3 N = normalize(input.normal.xyz);

  float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
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
