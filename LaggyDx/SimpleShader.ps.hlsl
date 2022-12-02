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
  float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
  textureColor *= diffuseColor;

  // DIFFUSE
  
  float lightAmount = saturate(dot(input.normal.xyz, -lightDirection));
  lightAmount = max(lightAmount, ambientStrength);
  textureColor.rgb *= lightAmount;
  
  // SPECULAR
  
  float3 reflection = reflect(lightDirection, input.normal.xyz);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb * specularIntensity);
  
  return textureColor;
}
