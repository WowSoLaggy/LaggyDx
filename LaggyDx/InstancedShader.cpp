#include "stdafx.h"
#include "InstancedShader.h"

#include "ICamera3.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "MemoryTexture.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "ShadowCamera.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

#include "Generated/Instanced.gen.vs.h"
#include "Generated/Simple.gen.ps.h"


namespace Dx
{
  InstancedShader::InstancedShader(const ICamera3& i_camera)
    : d_matrixBuffer(getRenderDevice(), sizeof(ViewProj))
    , d_cameraBuffer(getRenderDevice(), sizeof(CameraDesc))
    , d_lightBuffer(getRenderDevice(), sizeof(LightDesc))
    , d_shadowMatrixBuffer(getRenderDevice(), sizeof(ShadowCascadesDesc))
    , d_camera(i_camera)
    , d_emptyTexture(getResourceController().getTexture("white.png"))
  {
    std::fill(std::begin(d_shadowMapTextures), std::end(d_shadowMapTextures), &d_emptyTexture);

    getShaders().initVs(g_instancedVs, sizeof(g_instancedVs), getVertexLayoutPos3NormTextInstanced());
    getShaders().initPs(g_simplePs, sizeof(g_simplePs));
    getShaders().addSampler(true);
    getShaders().addComparisonSampler();
  }


  void InstancedShader::setLightDirection(Sdk::Vector3D i_direction)
  {
    d_lightDesc.lightDirection = getXmfloat3Norm(std::move(i_direction));
  }

  void InstancedShader::setLightColor(const Sdk::Vector4D& i_color)
  {
    d_lightDesc.lightColor = getXmfloat4(i_color);
  }

  void InstancedShader::setAmbientStrength(const double i_strength)
  {
    d_lightDesc.ambientStrength = (float)i_strength;
  }

  void InstancedShader::setShadowCascade(const int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map)
  {
    CONTRACT_EXPECT(i_cascade >= 0 && i_cascade < c_shadowCascadesCount);
    d_shadowCameras[i_cascade] = &i_camera;
    d_shadowMapTextures[i_cascade] = &i_map;
  }


  void InstancedShader::draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const
  {
    if (!isEnabled())
      return;

    if (!i_object.getVisible())
      return;

    const auto& instances = dynamic_cast<const InstanceBuffer&>(i_instances);
    if (instances.getInstanceCount() <= 0)
      return;

    setRenderStates();
    setShaders();
    setXfmMatrices();
    setCBuffers();
    setShadowCBuffer();
    setShadowTexture();

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh, instances);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setTexture(materialSpan.material);
        setMaterial(materialSpan.material);
        drawIndexedInstanced(materialSpan.count, materialSpan.startIndex, instances.getInstanceCount());
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void InstancedShader::setGeometryBuffers(const IMesh& i_mesh, const InstanceBuffer& i_instances) const
  {
    ID3D11Buffer* vbPtrs[2] = { i_mesh.getVertexBuffer().getPtr(), i_instances.getBuffer().getPtr() };
    const unsigned int strides[2] = {
      (unsigned int)i_mesh.getVertexBuffer().getStride(),
      (unsigned int)i_instances.getBuffer().getStride() };
    const unsigned int offsets[2] = { 0, 0 };

    getRenderDevice().getDeviceContextPtr()->IASetVertexBuffers(0, 2, vbPtrs, strides, offsets);
    getRenderDevice().getDeviceContextPtr()->IASetIndexBuffer(
      i_mesh.getIndexBuffer().getPtr(), DXGI_FORMAT_R32_UINT, 0);

    const auto topology = i_mesh.getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    getRenderDevice().getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

  void InstancedShader::setXfmMatrices() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (ViewProj*)mappedResource.pData;
    dataPtr->view = XMMatrixTranspose(d_camera.getViewMatrix());
    dataPtr->projection = XMMatrixTranspose(d_camera.getProjectionMatrix());

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void InstancedShader::setCBuffers() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_cameraBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (CameraDesc*)mappedResource.pData;
    dataPtr->cameraPos = getXmfloat3(d_camera.getPosition());

    getRenderDevice().getDeviceContextPtr()->Unmap(d_cameraBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(1, 1, d_cameraBuffer.getPp());
  }

  void InstancedShader::setShadowCBuffer() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_shadowMatrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (ShadowCascadesDesc*)mappedResource.pData;
    for (int i = 0; i < c_shadowCascadesCount; ++i)
    {
      // Identity fallback keeps receivers fully lit until a cascade is wired
      dataPtr->lightViewProj[i] = d_shadowCameras[i]
        ? XMMatrixTranspose(d_shadowCameras[i]->getViewMatrix() * d_shadowCameras[i]->getProjectionMatrix())
        : XMMatrixIdentity();
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_shadowMatrixBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(2, 1, d_shadowMatrixBuffer.getPp());
  }

  void InstancedShader::setShadowTexture() const
  {
    ID3D11ShaderResourceView* srvs[c_shadowCascadesCount];
    for (int i = 0; i < c_shadowCascadesCount; ++i)
      srvs[i] = d_shadowMapTextures[i]->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(1, c_shadowCascadesCount, srvs);
  }

  void InstancedShader::setTexture(const Material& i_material) const
  {
    auto* texturePtr = d_emptyTexture.getTexturePtr();

    if (!i_material.textureName.empty())
      texturePtr = getResourceController().getTexture(i_material.textureName).getTexturePtr();
    else if (i_material.texture)
      texturePtr = i_material.texture->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void InstancedShader::setMaterial(const Material& i_material) const
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
