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

// Explicit register: the cbuffers above rely on implicit b0/b1 slot assignment
cbuffer ShadowCBuffer : register(b2)
{
  matrix lightViewProj;
};


struct VertexInputType
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
};

// Must match PixelInputType of SimpleShader.ps.hlsl - the pixel shader is shared with InstancedShader
struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float4 shadowPos : TEXCOORD2;
};


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  // Directions shall ignore translations during xfms, hence w = 0.0f
  output.normal = normalize(mul(float4(input.normal.xyz, 0.0f), worldMatrix));
  
  // VIEW DIRECTION
  
  float4 worldPosition = mul(input.position, worldMatrix);
  output.viewDirection = normalize(cameraPos - worldPosition.xyz);

  // Light-space position for shadow-map lookup in the pixel shader
  output.shadowPos = mul(worldPosition, lightViewProj);

  return output;
}
