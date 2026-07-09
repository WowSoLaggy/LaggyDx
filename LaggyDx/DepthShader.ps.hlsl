Texture2D shaderTexture;
SamplerState SampleType;


struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
};


// Depth-only pass: alpha-clip so transparent texels don't cast shadows
void main(PixelInputType input)
{
  clip(shaderTexture.Sample(SampleType, input.tex).a - 0.5f);
}
