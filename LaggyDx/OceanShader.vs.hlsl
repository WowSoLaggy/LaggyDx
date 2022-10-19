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
  float4 waterColor;
  float2 _reserved;
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


PixelInputType main(VertexInputType input)
{
  PixelInputType output;

  // Change the position vector to be 4 units for proper matrix calculations.
  input.position.w = 1.0f;

  float3 offset = windDirection * windSpeed * time;

  input.position.y = sin(input.position.x + offset.x) * sin(input.position.z + offset.z);

  output.position = mul(input.position, worldMatrix);
  output.position = mul(output.position, viewMatrix);
  output.position = mul(output.position, projectionMatrix);

  output.normal.x = -cos(input.position.x + offset.x) * sin(input.position.z + offset.z);
  output.normal.y = 1;
  output.normal.z = -sin(input.position.x + offset.x) * cos(input.position.z + offset.z);
  output.normal = normalize(output.normal);

  output.tex = input.tex + offset.xz * textureCoef;

  return output;
}
