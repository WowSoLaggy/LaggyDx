#include "stdafx.h"
#include "SkydomeShader.h"

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

#include "Generated/Skydome.gen.ps.h"
#include "Generated/Skydome.gen.vs.h"


namespace Dx
{
  SkydomeShader::SkydomeShader(const ICamera3& i_camera)
    : d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_skyDomeBuffer(getRenderDevice(), sizeof(SkydomeSettings))
    , d_timeBuffer(getRenderDevice(), sizeof(TimeDesc))
    , d_windBuffer(getRenderDevice(), sizeof(WindDesc))
    , d_camera(i_camera)
    , d_mainTexture(getResourceController().getTexture("sky_main.png"))
    , d_horizonHazeTexture(getResourceController().getTexture("sky_horizon_haze.png"))
    , d_aroundSunTexture(getResourceController().getTexture("sky_around_sun.png"))
    , d_cloudsTexture1(getResourceController().getTexture("clouds1.png"))
    , d_cloudsTexture2(getResourceController().getTexture("clouds2.png"))
  {
    getShaders().initVs(g_skydomeVs, sizeof(g_skydomeVs), getVertexLayoutPos3NormText());
    getShaders().initPs(g_skydomePs, sizeof(g_skydomePs));
    getShaders().addSampler(false);
    getShaders().addSampler(true);
  }


  void SkydomeShader::setSunDirection(Sdk::Vector3D i_sunDir)
  {
    d_skyDomeSettings.sunDirection = getXmfloat3Norm(std::move(i_sunDir));
  }

  void SkydomeShader::setSunRadiusInternal(const float i_radius)
  {
    d_skyDomeSettings.sunRadiusInternal = i_radius;
  }

  void SkydomeShader::setSunRadiusExternal(const float i_radius)
  {
    d_skyDomeSettings.sunRadiusExternal = i_radius;
  }

  void SkydomeShader::setGlobalTime(const double i_time)
  {
    d_timeDesc.time = (float)i_time;
  }

  void SkydomeShader::setWindSpeed1(const double i_speed)
  {
    d_windDesc.speed1 = (float)i_speed;
  }

  void SkydomeShader::setWindSpeed2(const double i_speed)
  {
    d_windDesc.speed2 = (float)i_speed;
  }

  void SkydomeShader::setWindDirection1(Sdk::Vector2D i_windDir)
  {
    d_windDesc.direction1 = getXmfloat2Norm(std::move(i_windDir));
  }

  void SkydomeShader::setWindDirection2(Sdk::Vector2D i_windDir)
  {
    d_windDesc.direction2 = getXmfloat2Norm(std::move(i_windDir));
  }

  void SkydomeShader::setOvercast(const double i_overcast)
  {
    d_windDesc.overcast = (float)i_overcast;
  }

  void SkydomeShader::setCutoff(const double i_cutoff)
  {
    d_windDesc.cutoff = (float)i_cutoff;
  }


  void SkydomeShader::draw(const IObject3& i_object) const
  {
    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
    setTextures();

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials())
        drawIndexed(materialSpan.count, materialSpan.startIndex);
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void SkydomeShader::setRenderStates() const
  {
    ShaderBase::setRenderStates();

    auto depthStencilState = getRenderDevice().getDepthStencilState();
    depthStencilState.DepthEnable = FALSE;
    depthStencilState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    getRenderDevice().setDepthStencilState(std::move(depthStencilState));
  }


  void SkydomeShader::setGeometryBuffers(const IMesh& i_mesh) const
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

  void SkydomeShader::setXfmMatrices(const IObject3& i_object) const
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

  void SkydomeShader::setCBuffers() const
  {
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_skyDomeBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (SkydomeSettings*)mappedResource.pData;
      *dataPtr = d_skyDomeSettings;
      dataPtr->cameraPosition = getXmfloat3(d_camera.getPosition());

      getRenderDevice().getDeviceContextPtr()->Unmap(d_skyDomeBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(0, 1, d_skyDomeBuffer.getPp());
    }

    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_timeBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (TimeDesc*)mappedResource.pData;
      *dataPtr = d_timeDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_timeBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(1, 1, d_timeBuffer.getPp());
    }

    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_windBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (WindDesc*)mappedResource.pData;
      *dataPtr = d_windDesc;

      getRenderDevice().getDeviceContextPtr()->Unmap(d_windBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(2, 1, d_windBuffer.getPp());
    }
  }

  void SkydomeShader::setTextures() const
  {
    auto* textureMainPtr = d_mainTexture.getTexturePtr();
    auto* textureHorizonHazePtr = d_horizonHazeTexture.getTexturePtr();
    auto* textureAroundSunPtr = d_aroundSunTexture.getTexturePtr();
    auto* cloudsPtr1 = d_cloudsTexture1.getTexturePtr();
    auto* cloudsPtr2 = d_cloudsTexture2.getTexturePtr();

    std::vector<ID3D11ShaderResourceView*> textures {
      textureMainPtr, textureHorizonHazePtr, textureAroundSunPtr, cloudsPtr1, cloudsPtr2 };
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, (int)textures.size(), textures.data());
  }

} // ns Dx
