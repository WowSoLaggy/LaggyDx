SamplerState SampleType;

Texture2D skyMainTexture : register(t0);
Texture2D skyHorizonHazeTexture : register(t1);
Texture2D skyAroundSunTexture : register(t2);


cbuffer SkydomeSettings : register(b0)
{
  float3 cameraPosition;
  float sunRadiusInternal;
  float3 sunDirection;
  float sunRadiusExternal;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 posWorld : TEXCOORD1;
};


float getSunMask(float sunViewDot)
{
  float stepRadiusInternal = 1 - sunRadiusInternal * sunRadiusInternal;
  float stepRadiusExternal = 1 - sunRadiusExternal * sunRadiusExternal;
  return smoothstep(stepRadiusExternal, stepRadiusInternal, sunViewDot);
}


float4 main(PixelInputType input) : SV_TARGET
{
  float3 viewDirection = input.posWorld.xyz - cameraPosition;
  
  float sunViewDot = dot(sunDirection, viewDirection);
  float sunZenithDot = sunDirection.y;
  float viewZenithDot = viewDirection.y;
  
  float sunViewDot01 = (sunViewDot + 1.0f) * 0.5f;
  float sunZenithDot01 = (sunZenithDot + 1.0f) * 0.5f;
  
  float4 sunZenithColor = skyMainTexture.Sample(SampleType, float2(sunZenithDot01, 0.5f));
  float4 viewZenithColor = skyHorizonHazeTexture.Sample(SampleType, float2(sunZenithDot01, 0.5f));
  float4 sunViewColor = skyAroundSunTexture.Sample(SampleType, float2(sunZenithDot01, 0.5f));
  
  float vzMask = pow(saturate(1.0f - viewZenithDot), 4);
  float svMask = pow(saturate(sunViewDot), 4);
  
  float sunMask = getSunMask(sunViewDot);
  float4 sunColor = float4(1, 1, 1, 1) * sunMask;
  
  return sunZenithColor + vzMask * viewZenithColor + svMask * sunViewColor + sunColor;
}
