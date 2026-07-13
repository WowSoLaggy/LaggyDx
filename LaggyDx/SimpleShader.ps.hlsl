// Must match Dx::c_shadowCascadesCount
#define CascadesCount 3

Texture2D shaderTexture;
Texture2D shadowMap0 : register(t1);
Texture2D shadowMap1 : register(t2);
Texture2D shadowMap2 : register(t3);
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
  float4 shadowPos[CascadesCount] : TEXCOORD2;
};


// 3x3 PCF lookup in one cascade: 1 = fully lit, 0 = fully shadowed, -1 = pixel outside this cascade
float sampleCascade(Texture2D i_map, float4 i_shadowPos)
{
  float3 p = i_shadowPos.xyz / i_shadowPos.w;
  float2 uv = float2(p.x * 0.5 + 0.5, p.y * -0.5 + 0.5);

  float w, h;
  i_map.GetDimensions(w, h);
  const float2 texel = float2(1.0 / w, 1.0 / h);

  float result = -1.0;

  // Inset by the PCF radius so the kernel never reads past the cascade edge
  const float2 margin = texel * 2.0;
  if (all(uv >= margin) && all(uv <= 1.0 - margin) && p.z >= 0.0 && p.z <= 1.0)
  {
    float sum = 0.0;
    [unroll] for (int y = -1; y <= 1; ++y)
      [unroll] for (int x = -1; x <= 1; ++x)
        sum += i_map.SampleCmpLevelZero(ShadowSampler, uv + float2(x, y) * texel, p.z);

    result = sum / 9.0;
  }

  return result;
}

// Samples the first (sharpest) cascade containing the pixel: 1 = fully lit, 0 = fully shadowed
float getShadowFactor(float4 i_shadowPos[CascadesCount])
{
  float factor = sampleCascade(shadowMap0, i_shadowPos[0]);
  if (factor < 0.0)
    factor = sampleCascade(shadowMap1, i_shadowPos[1]);
  if (factor < 0.0)
    factor = sampleCascade(shadowMap2, i_shadowPos[2]);

  // Outside all cascades -> fully lit
  return factor < 0.0 ? 1.0 : factor;
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
