Texture2D shaderTexture;
SamplerState SampleType;


cbuffer SkydomeColorsCbuffer
{
  float4 colorZeroLevel;
  float4 colorTopLevel;
};


struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float height : TEXCOORD1;
};


float4 main(PixelInputType input) : SV_TARGET
{
  float alpha = saturate(input.height);
  return (1 - alpha) * colorZeroLevel + alpha * colorTopLevel;
}
