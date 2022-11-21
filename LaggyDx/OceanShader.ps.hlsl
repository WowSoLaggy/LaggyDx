SamplerState SampleType;

Texture2D objectTexture : register(t0);
Texture2D bumpTexture : register(t1);


cbuffer LightingCBuffer
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
  float specularPower;
  float3 _reserved;
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
  // NORMAL
  
  float4 normalMap = bumpTexture.Sample(SampleType, input.tex);
  float3 normal = normalMap.xzy * 2 - 1;
  normal = normalize(input.normal.xyz + normal);
  
  // DIFFUSE
  
  float4 textureColor = diffuseColor;
  float lightAmount = saturate(dot(normal, -lightDirection));
  textureColor.rgb *= lightAmount;
  
  // SPECULAR
  
  float3 reflection = reflect(lightDirection, normal);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb);
  
  //

  return textureColor;
}
