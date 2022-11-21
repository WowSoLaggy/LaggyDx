cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer TimeCBuffer
{
  float time;
};

cbuffer CameraCBuffer
{
  float3 cameraPos;
  float _reserved1;
};

cbuffer WaveCBuffer
{
  float4 wave1;
  float4 wave2;
  float4 wave3;
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
};

static const float PI = 3.14159265f;


float3 gerstnerWave(float4 wave, float3 pos, inout float3 tangent, inout float3 binormal)
{
  float steepness = wave.z;
  float waveLength = wave.w;
  
  float k = 2 * PI / waveLength;
  float c = sqrt(9.81 / k);
  float2 d = wave.xy;
  float f = k * (dot(d, pos.xz) + c * time);
  float a = steepness / k;
  
  tangent += float3(
    -d.x * d.x * steepness * sin(f),
    d.x * steepness * cos(f),
    -d.x * d.y * steepness * sin(f));
  binormal += float3(
    -d.x * d.y * steepness * sin(f),
    d.y * steepness * cos(f),
    -d.y * d.y * steepness * sin(f));
  
  return float3(
    d.x * a * cos(f),
    a * sin(f),
    d.y * a * cos(f));
}


PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  output.tex = input.tex;

  // VIEW DIRECTION
  
  float3 worldPosition = mul(input.position, worldMatrix).xyz;
  output.viewDirection = normalize(cameraPos - worldPosition.xyz);
  
  // WAVES
  
  float3 tangent = float3(1, 0, 0);
  float3 binormal = float3(0, 0, 1);
  
  worldPosition += gerstnerWave(wave1, worldPosition, tangent, binormal);
  worldPosition += gerstnerWave(wave2, worldPosition, tangent, binormal);
  worldPosition += gerstnerWave(wave3, worldPosition, tangent, binormal);

  // NORMAL

  float3 normal = normalize(cross(binormal, tangent));
  output.normal = float4(normal, 1.0f);

  // XFMS

  input.position.xyz = worldPosition;
  output.position = mul(input.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  return output;
}
