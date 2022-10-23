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
  float wavesSteepness;
  float wavesLength;
};

cbuffer WindCBuffer
{
  float2 windDirection;
  float2 _reserved;
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

static const float PI = 3.14159265f;

PixelInputType main(VertexInputType input)
{
  PixelInputType output;
  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;
  output.normal = input.normal;
  output.tex = input.tex;

  //

  float k = 2 * PI / wavesLength;
  float c = sqrt(9.81 / k);
  float f = k * (dot(windDirection, input.position.xz) + c * time);
  float a = wavesSteepness / k;
  float3 tangent = float3(
    1 - windDirection.x * windDirection.x * wavesSteepness * sin(f),
    windDirection.x * wavesSteepness * cos(f),
    -windDirection.x * windDirection.y * wavesSteepness * sin(f));
  float3 binormal = float3(
    -windDirection.x * windDirection.y * wavesSteepness * sin(f),
    windDirection.y * wavesSteepness * cos(f),
    1 - windDirection.y * windDirection.y * wavesSteepness * sin(f));
  float3 normal = normalize(cross(binormal, tangent));

  //

  input.position.x += windDirection.x * a * cos(f);
  input.position.y += a * sin(f);
  input.position.z += windDirection.y * a * cos(f);

  output.normal = normal;

  //

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  return output;
}
