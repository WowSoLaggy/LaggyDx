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
};

struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float3 worldPos : TEXCOORD2;
  float4 shadowPos[CascadesCount] : TEXCOORD3;
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

  // WORLD POSITION (also reused for the view direction below)
  // The terrain PS keys its texture blend off world height (worldPos.y) and
  // tiles each texture by world XZ (worldPos.xz), so the full world-space
  // position is passed through.

  float4 worldPosition = mul(input.position, worldMatrix);
  output.worldPos = worldPosition.xyz;

  // VIEW DIRECTION

  output.viewDirection = normalize(cameraPos - worldPosition.xyz);

  // Light-space position per cascade for the shadow-map lookup in the pixel shader
  [unroll] for (int i = 0; i < CascadesCount; ++i)
    output.shadowPos[i] = mul(worldPosition, lightViewProj[i]);

  return output;
}
