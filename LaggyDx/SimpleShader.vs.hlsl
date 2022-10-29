cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};


struct VertexInputType
{
  float4 position : POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
  float3 normal : NORMAL;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;
  output.normal = input.normal;

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  return output;
}
