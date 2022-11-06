#pragma once


namespace Dx
{
  struct MatrixCBuffer
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
  };

  struct GlobalCBuffer
  {
    float time{ 0 };
    float textureCoef{ 0 };
    XMFLOAT2 _reserved{ 0, 0 };
  };

  struct CameraCBuffer
  {
    XMFLOAT3 cameraPos{ 0, 0, 0 };
    float _reserved{ 0 };
  };

  struct WaveCBuffer
  {
    XMFLOAT4 wave1{ 0, 0, 0, 0 };
    XMFLOAT4 wave2{ 0, 0, 0, 0 };
    XMFLOAT4 wave3{ 0, 0, 0, 0 };
  };

  struct LightCBuffer
  {
    XMFLOAT4 diffuseColor{ 0, 0, 0 ,0 };
    XMFLOAT4 lightColor{ 0, 0, 0, 0 };
    XMFLOAT3 lightDirection{ 0, 0, 0 };
    float ambientStrength{ 0 };
    float specularPower{ 0 };
    XMFLOAT3 _reserved{ 0, 0, 0 };
  };

} // ns Dx
