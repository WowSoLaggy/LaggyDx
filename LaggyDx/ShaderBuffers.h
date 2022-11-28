#pragma once


namespace Dx
{
  struct WorldViewProj
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
  };

  struct TimeDesc
  {
    float time{ 0 };
    XMFLOAT3 _reserved{ 0, 0, 0 };
  };

  struct CameraDesc
  {
    XMFLOAT3 cameraPos{ 0, 0, 0 };
    float _reserved{ 0 };
  };

  struct WaveDesc
  {
    XMFLOAT4 wave1{ 0, 0, 0, 0 };
    XMFLOAT4 wave2{ 0, 0, 0, 0 };
    XMFLOAT4 wave3{ 0, 0, 0, 0 };
  };

  struct LightDesc
  {
    XMFLOAT4 diffuseColor{ 0, 0, 0 ,0 };
    XMFLOAT4 lightColor{ 0, 0, 0, 0 };
    XMFLOAT3 lightDirection{ 0, 0, 0 };
    float ambientStrength{ 0 };
    float specularPower{ 0 };
    XMFLOAT3 _reserved{ 0, 0, 0 };
  };

  struct TextureDisplacementDesc
  {
    XMFLOAT2 speed1{ 0, 0 };
    XMFLOAT2 speed2{ 0, 0 };
    float scale1{ 0 };
    float scale2{ 0 };
    XMFLOAT2 _reserved{ 0, 0 };
  };

} // ns Dx
