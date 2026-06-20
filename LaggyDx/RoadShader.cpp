#include "stdafx.h"
#include "RoadShader.h"

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

#include "Generated/Road.gen.ps.h"
#include "Generated/Road.gen.vs.h"


namespace Dx
{
  RoadShader::RoadShader(const ICamera3& i_camera)
    : d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_cameraBuffer(getRenderDevice(), sizeof(CameraDesc))
    , d_lightBuffer(getRenderDevice(), sizeof(LightDesc))
    , d_camera(i_camera)
    , d_roadTexture(getResourceController().getTexture("road.png"))
  {
    getShaders().initVs(g_roadVs, sizeof(g_roadVs), getVertexLayoutPos3NormText());
    getShaders().initPs(g_roadPs, sizeof(g_roadPs));
    getShaders().addSampler(true);
  }


  void RoadShader::setLightDirection(Sdk::Vector3D i_direction)
  {
    d_lightDesc.lightDirection = getXmfloat3Norm(std::move(i_direction));
  }

  void RoadShader::setLightColor(const Sdk::Vector4D& i_color)
  {
    d_lightDesc.lightColor = getXmfloat4(i_color);
  }

  void RoadShader::setAmbientStrength(const double i_strength)
  {
    d_lightDesc.ambientStrength = (float)i_strength;
  }


  void RoadShader::draw(const IObject3& i_object) const
  {
    if (!i_object.getVisible())
      return;

    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
    setTextures(i_object);

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setMaterial(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void RoadShader::setGeometryBuffers(const IMesh& i_mesh) const
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

  void RoadShader::setXfmMatrices(const IObject3& i_object) const
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

  void RoadShader::setCBuffers() const
  {
    // Camera
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_cameraBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (CameraDesc*)mappedResource.pData;
      dataPtr->cameraPos = getXmfloat3(d_camera.getPosition());

      getRenderDevice().getDeviceContextPtr()->Unmap(d_cameraBuffer.get(), 0);
      getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(1, 1, d_cameraBuffer.getPp());
    }
  }

  void RoadShader::setTextures(const IObject3& i_object) const
  {
    // Crossroad patches carry their own texture; roads fall back to the default.
    const ITexture* texture = i_object.getTexture();
    ID3D11ShaderResourceView* srv =
      texture ? texture->getTexturePtr() : d_roadTexture.getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &srv);
  }

  void RoadShader::setMaterial(const Material& i_material) const
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

} // ns Dx
