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
};


float4 main(PixelInputType input) : SV_TARGET
{
  float4 textureColor = diffuseColor;
  
  // Use this for picking from texture
  // textureColor = shaderTexture.Sample(SampleType, input.tex);

  float lightAmount = saturate(abs(dot(input.normal, -lightDirection)));
  textureColor.rgb *= lightAmount;

  return textureColor;
}
