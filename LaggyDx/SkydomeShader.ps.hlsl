SamplerState SampleType;

Texture2D skyMainTexture : register(t0);
Texture2D skyHorizonHazeTexture : register(t1);
Texture2D skyAroundSunTexture : register(t2);


cbuffer SkydomeColorsCbuffer : register(b0)
{
  float4 colorZeroLevel;
  float4 colorTopLevel;
};

cbuffer ViewSunDirsCBuffer : register(b1)
{
  float3 viewDirection;
  float3 sunDirection;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float height : TEXCOORD1;
};


float4 main(PixelInputType input) : SV_TARGET
{
  float sunViewDot = dot(sunDirection, viewDirection);
  float sunZenithDot = sunDirection.y;
  float viewZenithDot = viewDirection.y;
  
  float sunViewDot01 = (sunViewDot + 1.0f) * 0.5f;
  float sunZenithDot01 = (sunZenithDot + 1.0f) * 0.5f;
  
  float4 sunZenithColor = skyMainTexture.Sample(SampleType, float2(sunZenithDot01, 0.5f));
  
  return sunZenithColor;
}
