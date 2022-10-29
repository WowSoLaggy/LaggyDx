cbuffer MatrixBuffer
{
  matrix worldMatrix;
  matrix viewMatrix;
  matrix projectionMatrix;
};

cbuffer GlobalCBuffer
{
  float time;
  float textureCoef;
  float2 _reserved;
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

static const float PI = 3.14159265f;


float3 gerstnerWave(float4 wave, float3 p, inout float3 tangent, inout float3 binormal)
{
  float steepness = wave.z;
  float waveLength = wave.w;
  
  float k = 2 * PI / waveLength;
  float c = sqrt(9.81 / k);
  float2 d = wave.xy;
  float f = k * (dot(d, p.xz) + c * time);
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

  // WAVES
  
  float3 p = input.position.xyz;
  float3 tangent = float3(1, 0, 0);
  float3 binormal = float3(0, 0, 1);
  
  p += gerstnerWave(wave1, p, tangent, binormal);
  p += gerstnerWave(wave2, p, tangent, binormal);
  p += gerstnerWave(wave3, p, tangent, binormal);

  // NORMAL

  float3 normal = normalize(cross(binormal, tangent));
  output.normal = normal;

  // XFMS

  input.position.xyz = p;
  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);
  
  // VIEW DIRECTION
  
  float4 worldPosition = mul(input.position, worldMatrix);
  output.viewDirection = normalize(cameraPos - worldPosition.xyz);

  return output;
}
