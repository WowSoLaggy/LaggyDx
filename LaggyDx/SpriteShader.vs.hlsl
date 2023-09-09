cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};


struct VertexInputType
{
  float2 position : SV_POSITION;
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
  
  output.position = float4(input.position, 0.5f, 1);
  
  output.position = mul(output.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

	return output;
}