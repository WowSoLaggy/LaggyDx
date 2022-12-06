SamplerState samplerNoWrap : register(s0);
SamplerState samplerWrap : register(s1);

Texture2D skyMainTexture : register(t0);
Texture2D skyHorizonHazeTexture : register(t1);
Texture2D skyAroundSunTexture : register(t2);
Texture2D cloudsTexture : register(t3);


cbuffer SkydomeSettings : register(b0)
{
  float3 cameraPosition;
  float sunRadiusInternal;
  float3 sunDirection;
  float sunRadiusExternal;
};

cbuffer TimeDesc : register(b1)
{
  float time;
  float3 _reserved;
};

cbuffer WindDesc : register(b2)
{
  float2 windDirection;
  float windSpeed;
  float _reserved2;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 posWorld : TEXCOORD0;
  float2 tex : TEXCOORD1;
};


float getSunMask(float sunViewDot)
{
  float stepRadiusInternal = 1 - sunRadiusInternal * sunRadiusInternal;
  float stepRadiusExternal = 1 - sunRadiusExternal * sunRadiusExternal;
  return smoothstep(stepRadiusExternal, stepRadiusInternal, sunViewDot);
}


float4 main(PixelInputType input) : SV_TARGET
{
  float3 viewDirection = normalize(input.posWorld.xyz - cameraPosition);
  
  float sunViewDot = dot(sunDirection, viewDirection);
  float sunZenithDot = sunDirection.y;
  float viewZenithDot = viewDirection.y;
  
  float sunViewDot01 = (sunViewDot + 1.0f) * 0.5f;
  float sunZenithDot01 = (sunZenithDot + 1.0f) * 0.5f;
  
  float4 sunZenithColor = skyMainTexture.Sample(samplerNoWrap, float2(sunZenithDot01, 0.5f));
  float4 viewZenithColor = skyHorizonHazeTexture.Sample(samplerNoWrap, float2(sunZenithDot01, 0.5f));
  float4 sunViewColor = skyAroundSunTexture.Sample(samplerNoWrap, float2(sunZenithDot01, 0.5f));
  
  float vzMask = pow(saturate(1.0f - viewZenithDot), 4);
  float svMask = pow(saturate(sunViewDot), 4);
  
  float sunMask = getSunMask(sunViewDot);
  float4 sunColor = float4(1, 1, 1, 1) * sunMask;
  
  // Clouds
  
  float2 texCoords = input.tex + windDirection * windSpeed * time;
  float4 cloudsMap = cloudsTexture.Sample(samplerWrap, texCoords);
  return
    sunZenithColor +
    vzMask * viewZenithColor +
    svMask * sunViewColor +
    sunColor;
}
