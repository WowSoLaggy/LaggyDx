SamplerState SampleType;

Texture2D objectTexture : register(t0);
Texture2D bumpTexture : register(t1);
Texture2DMS<float4> depthTexture : register(t2);


cbuffer LightingCBuffer : register(b0)
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
  float specularIntensity;
  float specularPower;
  float3 _reserved;
};

cbuffer ViewportCBuffer : register(b1)
{
  float2 resolution;
  float nearPlane;
  float farPlane;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex1 : TEXCOORD0;
  float2 tex2 : TEXCOORD1;
  float3 viewDirection : TEXCOORD2;
};


float4 main(PixelInputType input) : SV_TARGET
{
  // NORMAL
  
  float4 normalMap1 = bumpTexture.Sample(SampleType, input.tex1);
  float4 normalMap2 = bumpTexture.Sample(SampleType, input.tex2);
  float3 normal1 = normalMap1.xzy * 2 - 1;
  float3 normal2 = normalMap2.xzy * 2 - 1;
  float3 normal = normalize(input.normal.xyz + (normal1 + normal2) * 1);
  
  // DEPTH
  
  float2 screenCoords = float2(
    input.position.x / resolution.x,
    input.position.y / resolution.y);
  float depth = depthTexture.Load(input.position.xy, 0).r;
  float alpha = pow(depth, 64);
  alpha = max(alpha, 0.2);
  
  // DIFFUSE
  
  float4 textureColor = diffuseColor;
  textureColor.a = alpha;
  float lightAmount = saturate(dot(normal, -lightDirection));
  lightAmount = max(lightAmount, ambientStrength);
  textureColor.rgb *= lightAmount;
  
  // SPECULAR
  
  float3 reflection = reflect(lightDirection, normal);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb * specularIntensity);
  
  //

  return textureColor;
}
