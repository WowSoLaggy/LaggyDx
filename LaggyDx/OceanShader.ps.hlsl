Texture2D shaderTexture;
SamplerState SampleType;


cbuffer LightingCBuffer
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
  float3 viewDirection : TEXCOORD1;
};


float4 main(PixelInputType input) : SV_TARGET
{
  float4 textureColor = diffuseColor;
  
  // Use this for picking from texture
  //textureColor = shaderTexture.Sample(SampleType, input.tex);

  // DIFFUSE
  
  float lightAmount = saturate(dot(input.normal, -lightDirection));
  textureColor.rgb *= lightAmount;
  
  // SPECULAR
  
  float3 reflection = reflect(lightDirection, input.normal);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, 32);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb);
  
  //

  return textureColor;
}
