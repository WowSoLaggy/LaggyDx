#include "stdafx.h"
#include "SimpleShader.h"

#include "ICamera.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "TextureResource.h"

#include "Generated/Simple.gen.ps.h"
#include "Generated/Simple.gen.vs.h"


namespace Dx
{
  SimpleShader::SimpleShader(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
    : ISimpleShader(i_renderDevice)
    , d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_cameraBuffer(getRenderDevice(), sizeof(CameraDesc))
    , d_lightBuffer(getRenderDevice(), sizeof(LightDesc))
    , d_camera(i_camera)
    , d_resourceController(i_resourceController)
    , d_emptyTexture(i_resourceController.getTexture("white.png"))
  {
    createShaders();
  }

  SimpleShader::~SimpleShader()
  {
    disposeShaders();
  }


  void SimpleShader::setLightDirection(Sdk::Vector3D i_direction)
  {
    d_lightDesc.lightDirection = getXmfloat3Norm(std::move(i_direction));
  }

  void SimpleShader::setLightColor(const Sdk::Vector4D& i_color)
  {
    d_lightDesc.lightColor = getXmfloat4(i_color);
  }

  void SimpleShader::setAmbientStrength(const double i_strength)
  {
    d_lightDesc.ambientStrength = (float)i_strength;
  }


  void SimpleShader::draw(const IObject3& i_object) const
  {
    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
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


  void SimpleShader::createShaders()
  {
    getShaders().initVs(g_simpleVs, sizeof(g_simpleVs));
    getShaders().initPs(g_simplePs, sizeof(g_simplePs));


    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hRes = getRenderDevice().getDevicePtr()->CreateSamplerState(&samplerDesc, &d_sampleState);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_sampleState != nullptr);
  }

  void SimpleShader::disposeShaders()
  {
    d_sampleState->Release();
    d_sampleState = nullptr;
  }


  void SimpleShader::setShaders() const
  {
    getRenderDevice().getDeviceContextPtr()->IASetInputLayout(getShaders().getLayout());
    getRenderDevice().getDeviceContextPtr()->VSSetShader(getShaders().getVs(), nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetShader(getShaders().getPs(), nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetSamplers(0, 1, &d_sampleState);
  }

  void SimpleShader::setGeometryBuffers(const IMesh& i_mesh) const
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

  void SimpleShader::setXfmMatrices(const IObject3& i_object) const
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

  void SimpleShader::setCBuffers() const
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

  void SimpleShader::setTexture(const IObject3& i_object) const
  {
    auto* texturePtr = static_cast<const TextureResource&>(d_emptyTexture).getTexturePtr();

    if (const auto* textureResource = dynamic_cast<const TextureResource*>(i_object.getTextureResource()))
      texturePtr = textureResource->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void SimpleShader::setTexture(const Material& i_material) const
  {
    if (!i_material.textureName.empty())
    {
      const auto& texture = dynamic_cast<const TextureResource&>(
        d_resourceController.getTexture(i_material.textureName));
      auto* texturePtr = texture.getTexturePtr();

      getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
  }

  void SimpleShader::setMaterial(const Material& i_material) const
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
    dataPtr->specularPower = i_material.specularPower;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_lightBuffer.get(), 0);
    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(0, 1, d_lightBuffer.getPp());
  }

  void SimpleShader::drawIndexed(const int i_count, const int i_startIndex) const
  {
    getRenderDevice().getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx
