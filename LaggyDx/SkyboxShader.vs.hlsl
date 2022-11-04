cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer CameraCBuffer
{
  float3 cameraPos;
  float _reserved1;
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
  float3 viewDirection : TEXCOORD1;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;
  output.normal = input.normal;

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  // VIEW DIRECTION
  
  float4 worldPosition = mul(input.position, worldMatrix);
  output.viewDirection = normalize(cameraPos - worldPosition.xyz);
  
  return output;
}
