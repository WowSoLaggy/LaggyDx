#include "stdafx.h"
#include "SkydomeShader.h"

#include "ICamera.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "Texture.h"
#include "VertexLayout.h"

#include "Generated/Skydome.gen.ps.h"
#include "Generated/Skydome.gen.vs.h"


namespace Dx
{
  SkydomeShader::SkydomeShader(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
    : ISkydomeShader(i_renderDevice)
    , d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_skyDomeBuffer(getRenderDevice(), sizeof(SkydomeSettings))
    , d_camera(i_camera)
    , d_resourceController(i_resourceController)
    , d_mainTexture(i_resourceController.getTexture("sky_main.png"))
    , d_horizonHazeTexture(i_resourceController.getTexture("sky_horizon_haze.png"))
    , d_aroundSunTexture(i_resourceController.getTexture("sky_around_sun.png"))
  {
    getShaders().initVs(g_skydomeVs, sizeof(g_skydomeVs));
    getShaders().initPs(g_skydomePs, sizeof(g_skydomePs));
    getShaders().initSampler(false);
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
    Shader::setRenderStates();

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
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(1, 1, d_skyDomeBuffer.getPp());
    }
  }

  void SkydomeShader::setTextures() const
  {
    auto* textureMainPtr = static_cast<const Texture&>(d_mainTexture).getTexturePtr();
    auto* textureHorizonHazePtr = static_cast<const Texture&>(d_horizonHazeTexture).getTexturePtr();
    auto* textureAroundSunPtr = static_cast<const Texture&>(d_aroundSunTexture).getTexturePtr();

    ID3D11ShaderResourceView* textures[] = { textureMainPtr, textureHorizonHazePtr, textureAroundSunPtr };
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 3, textures);
  }

  void SkydomeShader::drawIndexed(const int i_count, const int i_startIndex) const
  {
    getRenderDevice().getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx
