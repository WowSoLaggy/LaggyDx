cbuffer MatrixBuffer
{
  matrix worldMatrix;
};


struct VertexInputType
{
  float4 position : POSITION;
  float2 tex : TEXCOORD0;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;
  //output.position = mul(input.position, worldMatrix);
  output.position = input.position;

	return output;
}