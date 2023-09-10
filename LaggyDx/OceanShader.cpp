#include "stdafx.h"
#include "OceanShader.h"

#include "ICamera3.h"
#include "IndexBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

#include "Generated/Ocean.gen.ps.h"
#include "Generated/Ocean.gen.vs.h"


namespace Dx
{
  OceanShader::OceanShader(const ICamera3& i_camera)
    : d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_cameraBuffer(getRenderDevice(), sizeof(CameraDesc))
    , d_lightBuffer(getRenderDevice(), sizeof(LightDesc))
    , d_depthFogBuffer(getRenderDevice(), sizeof(DepthFogDesc))
    , d_viewportBuffer(getRenderDevice(), sizeof(ViewportDesc))
    , d_timeBuffer(getRenderDevice(), sizeof(TimeDesc))
    , d_waveBuffer(getRenderDevice(), sizeof(WaveDesc))
    , d_texturesDisplacementBuffer(getRenderDevice(), sizeof(TextureDisplacementDesc))
    , d_camera(i_camera)
    , d_emptyTexture(getResourceController().getTexture("white.png"))
    , d_bumpTexture(getResourceController().getTexture("bump.png"))
  {
    getShaders().initVs(g_oceanVs, sizeof(g_oceanVs), getVertexLayoutPos3NormText());
    getShaders().initPs(g_oceanPs, sizeof(g_oceanPs));
    getShaders().addSampler(true);
  }


  void OceanShader::setGlobalTime(const double i_time)
  {
    d_timeDesc.time = (float)i_time;
  }


  void OceanShader::setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction)
  {
    const auto normalizedDirection = getXmfloat2Norm(std::move(i_direction));
    auto& wave = getWaveByIndex(i_waveIndex);
    wave.x = normalizedDirection.x;
    wave.y = normalizedDirection.y;
  }

  void OceanShader::setWavesSteepness(int i_waveIndex, const double i_steepness)
  {
    auto& wave = getWaveByIndex(i_waveIndex);
    wave.z = (float)i_steepness;
  }

  void OceanShader::setWavesLength(int i_waveIndex, const double i_length)
  {
    auto& wave = getWaveByIndex(i_waveIndex);
    wave.w = (float)i_length;
  }


  void OceanShader::setLightDirection(Sdk::Vector3D i_direction)
  {
    d_lightDesc.lightDirection = getXmfloat3Norm(std::move(i_direction));
  }

  void OceanShader::setLightColor(const Sdk::Vector4D& i_color)
  {
    d_lightDesc.lightColor = getXmfloat4(i_color);
  }

  void OceanShader::setAmbientStrength(const double i_strength)
  {
    d_lightDesc.ambientStrength = (float)i_strength;
  }


  void OceanShader::setFogDepthStart(const double i_depthStart)
  {
    d_depthFogDesc.depthStart = (float)i_depthStart;
  }

  void OceanShader::setFogDepthEnd(const double i_depthEnd)
  {
    d_depthFogDesc.depthEnd = (float)i_depthEnd;

  }

  void OceanShader::setFogMinPower(const double i_minPower)
  {
    d_depthFogDesc.powerMin = (float)i_minPower;
  }

  void OceanShader::setFogMaxPower(const double i_maxPower)
  {
    d_depthFogDesc.powerMax = (float)i_maxPower;
  }


  void OceanShader::setTexturesDisplacementSettings(
    const double i_scale1, const double i_scale2,
    const Sdk::Vector2D& i_speed1, const Sdk::Vector2D& i_speed2)
  {
    d_texturesDisplacementDesc.scale1 = (float)i_scale1;
    d_texturesDisplacementDesc.scale2 = (float)i_scale2;
    d_texturesDisplacementDesc.speed1 = getXmfloat2(i_speed1);
    d_texturesDisplacementDesc.speed2 = getXmfloat2(i_speed2);
  }


  void OceanShader::draw(const IObject3& i_object) const
  {
    getRenderDevice().prepareDepthBufferTexture();

    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
    setCommonTextures();
    setTexture(i_object);

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setTexture(materialSpan.material);
        setMaterial(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void OceanShader::setGeometryBuffers(const IMesh& i_mesh) const
  {
    auto* vbPtr = i_mesh.getVertexBuffer().getPtr();
    unsigned int stride = i_mesh.getVertexBuffer().getStride();
    auto* ibPtr = i_mesh.getIndexBuffer().getPtr();
    unsigned int offset = 0;

    getRenderDevice().getDeviceContextPtr()->IASetVertexBuffers(0, 1, &vbPtr, &stride, &offset);
    getRenderDevice().getDeviceContextPtr()->IASetIndexBuffer(ibPtr, DXGI_FORMAT_R32_UINT, 0);

    const auto topology = i_mesh.getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    getRenderDevice().getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

  void OceanShader::setXfmMatrices(const IObject3& i_object) const
  {
    auto getWorldMatrixTransposed = [&]()
    {
      const auto& position = i_object.getPosition();
      const auto& rotation = i_object.getRotation();
      const auto& scale = i_object.getScale();
      const auto worldMatrix =
        XMMatrixScaling(scale.x, scale.y, scale.z) *
        XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
        XMMatrixTranslation(position.x, position.y, position.z);

      return XMMatrixTranspose(worldMatrix);
    };

    const auto worldMatrix = getWorldMatrixTransposed();
    const auto viewMatrix = XMMatrixTranspose(d_camera.getViewMatrix());
    const auto projectionMatrix = XMMatrixTranspose(d_camera.getProjectionMatrix());

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (WorldViewProj*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void OceanShader::setCBuffers() const
  {
    // Global
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_timeBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (TimeDesc*)mappedResource.pData;
      *dataPtr = d_timeDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_timeBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(1, 1, d_timeBuffer.getPp());
    }

    // Camera
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_cameraBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (CameraDesc*)mappedResource.pData;
      dataPtr->cameraPos = getXmfloat3(d_camera.getPosition());

      getRenderDevice().getDeviceContextPtr()->Unmap(d_cameraBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(2, 1, d_cameraBuffer.getPp());
    }

    // Wind
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_waveBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (WaveDesc*)mappedResource.pData;
      *dataPtr = d_waveDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_waveBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(3, 1, d_waveBuffer.getPp());
    }

    // Textures displacement
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      HRESULT hRes = getRenderDevice().getDeviceContextPtr()->Map(
        d_texturesDisplacementBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      CONTRACT_ASSERT(!FAILED(hRes));

      auto* dataPtr = (TextureDisplacementDesc*)mappedResource.pData;
      *dataPtr = d_texturesDisplacementDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_texturesDisplacementBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(4, 1, d_texturesDisplacementBuffer.getPp());
    }

    // Viewport
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      HRESULT hRes = getRenderDevice().getDeviceContextPtr()->Map(
        d_viewportBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      CONTRACT_ASSERT(!FAILED(hRes));

      auto* dataPtr = (ViewportDesc*)mappedResource.pData;
      dataPtr->resolution = XMFLOAT2(
        (float)getRenderDevice().getDepthBufferTexture().getTextureDesc().Width,
        (float)getRenderDevice().getDepthBufferTexture().getTextureDesc().Height);
      dataPtr->nearPlane = d_camera.getViewportMinZ();
      dataPtr->farPlane = d_camera.getViewportMaxZ();

      getRenderDevice().getDeviceContextPtr()->Unmap(d_viewportBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(1, 1, d_viewportBuffer.getPp());
    }

    // Depth fog
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      HRESULT hRes = getRenderDevice().getDeviceContextPtr()->Map(
        d_depthFogBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
      CONTRACT_ASSERT(!FAILED(hRes));

      auto* dataPtr = (DepthFogDesc*)mappedResource.pData;
      *dataPtr = d_depthFogDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_depthFogBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(2, 1, d_depthFogBuffer.getPp());
    }
  }

  void OceanShader::setCommonTextures() const
  {
    auto* bumpTexturePtr = d_bumpTexture.getTexturePtr();
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(1, 1, &bumpTexturePtr);

    auto* depthTexturePtr = getRenderDevice().getDepthBufferTexture().getTexturePtr();
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(2, 1, &depthTexturePtr);
  }

  void OceanShader::setTexture(const IObject3& i_object) const
  {
    auto* texturePtr = d_emptyTexture.getTexturePtr();

    if (const auto* texture = i_object.getTexture())
      texturePtr = texture->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void OceanShader::setTexture(const Material& i_material) const
  {
    if (!i_material.textureName.empty())
    {
      const auto& texture = getResourceController().getTexture(i_material.textureName);
      auto* texturePtr = texture.getTexturePtr();

      getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
  }

  void OceanShader::setMaterial(const Material& i_material) const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_lightBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (LightDesc*)mappedResource.pData;
    *dataPtr = d_lightDesc;
    dataPtr->diffuseColor = XMFLOAT4(
      i_material.diffuseColor.x,
      i_material.diffuseColor.y,
      i_material.diffuseColor.z,
      i_material.diffuseColor.w);
    dataPtr->specularIntensity = i_material.specularIntensity;
    dataPtr->specularPower = i_material.specularPower;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_lightBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(0, 1, d_lightBuffer.getPp());
  }


  XMFLOAT4& OceanShader::getWaveByIndex(int i_waveIndex)
  {
    if (i_waveIndex == 0)
      return d_waveDesc.wave1;
    else if (i_waveIndex == 1)
      return d_waveDesc.wave2;
    else if (i_waveIndex == 2)
      return d_waveDesc.wave3;

    CONTRACT_EXPECT(false);
  }

} // ns Dx
