cbuffer MatrixBuffer
{
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer CameraCBuffer
{
  float3 cameraPos;
  float _reserved1;
};

// Must match Dx::c_shadowCascadesCount
#define CascadesCount 3

// Explicit register: the cbuffers above rely on implicit b0/b1 slot assignment
cbuffer ShadowCBuffer : register(b2)
{
  matrix lightViewProj[CascadesCount];
};


struct VertexInputType
{
  float4 position : POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float4 instPosRot : TEXCOORD1; // Per-instance: xyz - world position, w - rotation around Y
  float instScale : TEXCOORD2;   // Per-instance: uniform scale
};

// Must match PixelInputType of SimpleShader.ps.hlsl - the pixel shader is shared
struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float4 shadowPos[CascadesCount] : TEXCOORD2;
};


// Rotation around Y matching the row-vector convention of XMMatrixRotationY
float3 rotateY(float3 v, float s, float c)
{
  return float3(v.x * c + v.z * s, v.y, -v.x * s + v.z * c);
}


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;

  float s, c;
  sincos(input.instPosRot.w, s, c);

  float3 worldPosition = rotateY(input.position.xyz * input.instScale, s, c) + input.instPosRot.xyz;

  output.position = mul(float4(worldPosition, 1.0f), viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  output.normal = float4(normalize(rotateY(input.normal.xyz, s, c)), 0.0f);

  output.viewDirection = normalize(cameraPos - worldPosition);

  // Light-space position per cascade for the shadow-map lookup in the pixel shader
  [unroll] for (int i = 0; i < CascadesCount; ++i)
    output.shadowPos[i] = mul(float4(worldPosition, 1.0f), lightViewProj[i]);

  return output;
}
