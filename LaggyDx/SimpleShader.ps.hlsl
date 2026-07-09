Texture2D shaderTexture;
Texture2D shadowMap : register(t1);
SamplerState SampleType;
SamplerComparisonState ShadowSampler : register(s1);


cbuffer LightingCBuffer
{
  float4 diffuseColor;
  float4 lightColor;
  float3 lightDirection;
  float ambientStrength;
  float specularIntensity;
  float specularPower;
  float2 _reserved;
};


// Must match PixelInputType of SimpleShader.vs.hlsl and InstancedShader.vs.hlsl - both feed this shader
struct PixelInputType
{
  float4 position : SV_POSITION;
  float4 normal : NORMAL;
  float2 tex : TEXCOORD0;
  float3 viewDirection : TEXCOORD1;
  float4 shadowPos : TEXCOORD2;
};


// 3x3 PCF shadow-map lookup: 1 = fully lit, 0 = fully shadowed
float getShadowFactor(float4 i_shadowPos)
{
  float3 p = i_shadowPos.xyz / i_shadowPos.w;
  float2 uv = float2(p.x * 0.5 + 0.5, p.y * -0.5 + 0.5);

  // Outside the light's ortho box -> fully lit
  float result = 1.0;

  if (all(saturate(uv) == uv) && p.z >= 0.0 && p.z <= 1.0)
  {
    float w, h;
    shadowMap.GetDimensions(w, h);
    const float2 texel = float2(1.0 / w, 1.0 / h);

    float sum = 0.0;
    [unroll] for (int y = -1; y <= 1; ++y)
      [unroll] for (int x = -1; x <= 1; ++x)
        sum += shadowMap.SampleCmpLevelZero(ShadowSampler, uv + float2(x, y) * texel, p.z);

    result = sum / 9.0;
  }

  return result;
}


float4 main(PixelInputType input) : SV_TARGET
{
  // The VS normal is unit-length at each vertex, but linear interpolation
  // across the triangle shortens it — re-normalize per pixel so diffuse and
  // (especially) specular don't darken/wobble toward triangle centers.
  float3 N = normalize(input.normal.xyz);

  float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
  textureColor *= diffuseColor;

  // Alpha test: discard transparent pixels so they don't write depth and punch holes into objects behind
  clip(textureColor.a - 0.5f);

  // DIFFUSE

  float shadow = getShadowFactor(input.shadowPos);
  float lightAmount = saturate(dot(N, -lightDirection)) * shadow;
  lightAmount = max(lightAmount, ambientStrength);
  textureColor.rgb *= lightAmount;

  // SPECULAR

  float3 reflection = reflect(lightDirection, N);
  float dotProduct = saturate(dot(reflection, input.viewDirection));
  float specularValue = pow(dotProduct, specularPower);
  float4 specular = float4(specularValue, specularValue, specularValue, 1.0);
  textureColor.rgb = saturate(textureColor.rgb + specular.rgb * specularIntensity * shadow);

  return textureColor;
}
