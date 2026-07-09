cbuffer MatrixBuffer
{
  matrix viewMatrix;
  matrix projectionMatrix;
};


struct VertexInputType
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float4 instPosRot : TEXCOORD1; // Per-instance: xyz - world position, w - rotation around Y
  float instScale : TEXCOORD2;   // Per-instance: uniform scale
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
};


// Rotation around Y matching the row-vector convention of XMMatrixRotationY
float3 rotateY(float3 v, float s, float c)
{
  return float3(v.x * c + v.z * s, v.y, -v.x * s + v.z * c);
}


// Depth-only pass: light view/projection ride in the view/proj slots
PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;

  float s, c;
  sincos(input.instPosRot.w, s, c);

  float3 worldPosition = rotateY(input.position.xyz * input.instScale, s, c) + input.instPosRot.xyz;

  output.position = mul(float4(worldPosition, 1.0f), viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  return output;
}
