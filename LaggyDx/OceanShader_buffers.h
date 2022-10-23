#pragma once


namespace Dx
{
  struct LightingCBuffer
  {
    XMFLOAT4 diffuseColor{ 0, 0, 0 ,0 };
    XMFLOAT4 lightColor{ 0, 0, 0, 0 };
    XMFLOAT3 lightDirection{ 0, 0, 0 };
    float ambientStrength{ 0 };
  };

  struct GlobalCBuffer
  {
    float time{ 0 };
    float textureCoef{ 0 };
    XMFLOAT2 _reserved{ 0, 0 };
  };

  struct WaveCBuffer
  {
    XMFLOAT4 wave1{ 0, 0, 0, 0 };
    XMFLOAT4 wave2{ 0, 0, 0, 0 };
    XMFLOAT4 wave3{ 0, 0, 0, 0 };
  };

} //ns Dx
