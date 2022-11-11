cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};


struct VertexInputType
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 posWorld : TEXCOORD1;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;

  output.position = mul(input.position, worldMatrix);
  output.posWorld = output.position;
  
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  return output;
}
