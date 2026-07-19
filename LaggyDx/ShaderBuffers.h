#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  struct WorldMatrix
  {
    XMMATRIX world;
  };

  struct WorldViewProj
  {
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
  };

  struct ViewProj
  {
    XMMATRIX view;
    XMMATRIX projection;
  };

  struct ShadowCascadesDesc
  {
    XMMATRIX lightViewProj[c_shadowCascadesCount];
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
    float specularIntensity{ 0 };
    float specularPower{ 0 };
    XMFLOAT2 _reserved{ 0, 0 };
  };

  struct GridDesc
  {
    float cellSize{ 0 };
    XMFLOAT3 _reserved{ 0, 0, 0 };
  };

  struct TerrainPaintLayerDesc
  {
    float tile{ 1 };
    float tileCoarse{ 1 };
    float coarseMix{ 0 };
    float triplanar{ 0 }; // > 0.5 = triplanar sampling
    XMFLOAT3 tint{ 1, 1, 1 };
    float enabled{ 0 };   // 0 mutes the layer regardless of its mask
  };

  struct TerrainPaintDesc
  {
    XMFLOAT2 invMapSize{ 0, 0 }; // 1 / map world size, maps worldPos.xz to paint-mask UVs
    XMFLOAT2 _reserved{ 0, 0 };
    TerrainPaintLayerDesc layers[c_maxTerrainPaintLayers];
  };

  struct DepthFogDesc
  {
    float depthStart{ 0 };
    float depthEnd{ 0 };
    float powerMin{ 0 };
    float powerMax{ 0 };
  };

  struct WindDesc
  {
    XMFLOAT2 direction1{ 0, 0 };
    XMFLOAT2 direction2{ 0, 0 };
    float speed1{ 0 };
    float speed2{ 0 };
    float overcast{ 0 };
    float cutoff{ 0 };
  };

  struct ViewportDesc
  {
    XMFLOAT2 resolution{ 0, 0 };
    float nearPlane{ 0 };
    float farPlane{ 0 };
  };

  struct TextureDisplacementDesc
  {
    XMFLOAT2 speed1{ 0, 0 };
    XMFLOAT2 speed2{ 0, 0 };
    float scale1{ 0 };
    float scale2{ 0 };
    XMFLOAT2 _reserved{ 0, 0 };
  };

  struct UvOffsetDesc
  {
    XMFLOAT2 offset{ 0, 0 };
    XMFLOAT2 multiplier{ 1, 1 };
  };

  struct ColorDesc
  {
    XMFLOAT4 color{ 1, 1, 1, 1 };
  };

} // ns Dx
