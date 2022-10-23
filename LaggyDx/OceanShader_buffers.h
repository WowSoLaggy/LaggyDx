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
    float wavesSteepness{ 0 };
    float wavesLength{ 0 };
  };

  struct WindCBuffer
  {
    XMFLOAT3 direction{ 0, 0, 0 };
    float speed{ 0 };
  };

} //ns Dx
