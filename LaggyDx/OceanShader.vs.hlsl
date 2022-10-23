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
  float3 windDirection;
  float windSpeed;
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
  float f = k * (input.position.x + windSpeed * time);
  float a = wavesSteepness / k;
  float3 tangent = normalize(float3(
    1 - wavesSteepness * sin(f),
    wavesSteepness * cos(f),
    0));

  //

  input.position.x += a * cos(f);
  input.position.y += a * sin(f);

  output.normal = float3(-tangent.y, tangent.x, 0);

  //

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  return output;
}
