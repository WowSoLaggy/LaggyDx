#include "stdafx.h"
#include "SkydomeShader.h"

#include "ICamera.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShadersUtils.h"
#include "TextureResource.h"
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
    , d_camera(i_camera)
    , d_resourceController(i_resourceController)
    , d_mainTexture(i_resourceController.getTexture("sky_main.png"))
    , d_horizonHazeTexture(i_resourceController.getTexture("sky_horizon_haze.png"))
    , d_aroundSunTexture(i_resourceController.getTexture("sky_around_sun.png"))
  {
    createShaders();
    createBuffers();
  }

  SkydomeShader::~SkydomeShader()
  {
    disposeBuffers();
    disposeShaders();
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


  void SkydomeShader::createShaders()
  {
    // PS

    HRESULT hRes = getRenderDevice().getDevicePtr()->CreatePixelShader(
      g_skydomePs, sizeof(g_skydomePs), NULL, &d_pixelShader);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_pixelShader != nullptr);

    // Sampler state

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hRes = getRenderDevice().getDevicePtr()->CreateSamplerState(&samplerDesc, &d_sampleState);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_sampleState != nullptr);

    // VS

    hRes = getRenderDevice().getDevicePtr()->CreateVertexShader(
      g_skydomeVs, sizeof(g_skydomeVs), NULL, &d_vertexShader);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_vertexShader != nullptr);

    // Input layout

    const auto& layout = getVertexLayout();
    hRes = getRenderDevice().getDevicePtr()->CreateInputLayout(layout.data(), (int)layout.size(),
      g_skydomeVs, sizeof(g_skydomeVs), &d_layout);
    CONTRACT_ASSERT(!FAILED(hRes));
    CONTRACT_ASSERT(d_layout != nullptr);
  }

  void SkydomeShader::disposeShaders()
  {
    // VS

    d_layout->Release();
    d_layout = nullptr;

    d_vertexShader->Release();
    d_vertexShader = nullptr;

    // PS

    d_sampleState->Release();
    d_sampleState = nullptr;

    d_pixelShader->Release();
    d_pixelShader = nullptr;
  }


  void SkydomeShader::createBuffers()
  {
    auto createBuffer = [&](const int i_sizeOf, ID3D11Buffer** i_buf)
    {
      D3D11_BUFFER_DESC desc;
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.ByteWidth = i_sizeOf;
      desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.StructureByteStride = 0;

      HRESULT hRes = getRenderDevice().getDevicePtr()->CreateBuffer(&desc, nullptr, i_buf);
      CONTRACT_ASSERT(!FAILED(hRes));
    };

    createBuffer(sizeof(MatrixCBuffer), &d_matrixBuffer);
    createBuffer(sizeof(SkydomeSettings), &d_skyDomeBuffer);
  }

  void SkydomeShader::disposeBuffers()
  {
    auto releaseBuffer = [](ID3D11Buffer** i_buf)
    {
      (*i_buf)->Release();
      *i_buf = nullptr;
    };

    releaseBuffer(&d_skyDomeBuffer);
    releaseBuffer(&d_matrixBuffer);
  }


  void SkydomeShader::setRenderStates() const
  {
    Shader::setRenderStates();
    getRenderDevice().setDepthEnabled(false);
  }

  void SkydomeShader::setShaders() const
  {
    getRenderDevice().getDeviceContextPtr()->IASetInputLayout(d_layout);
    getRenderDevice().getDeviceContextPtr()->VSSetShader(d_vertexShader, nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetShader(d_pixelShader, nullptr, 0);
    getRenderDevice().getDeviceContextPtr()->PSSetSamplers(0, 1, &d_sampleState);
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
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (MatrixCBuffer*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer, 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, &d_matrixBuffer);
  }

  void SkydomeShader::setCBuffers() const
  {
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_skyDomeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (SkydomeSettings*)mappedResource.pData;
      *dataPtr = d_skyDomeSettings;
      dataPtr->cameraPosition = getXmfloat3(d_camera.getPosition());

      getRenderDevice().getDeviceContextPtr()->Unmap(d_skyDomeBuffer, 0);
      getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(1, 1, &d_skyDomeBuffer);
    }
  }

  void SkydomeShader::setTextures() const
  {
    auto* textureMainPtr = static_cast<const TextureResource&>(d_mainTexture).getTexturePtr();
    auto* textureHorizonHazePtr = static_cast<const TextureResource&>(d_horizonHazeTexture).getTexturePtr();
    auto* textureAroundSunPtr = static_cast<const TextureResource&>(d_aroundSunTexture).getTexturePtr();

    ID3D11ShaderResourceView* textures[] = { textureMainPtr, textureHorizonHazePtr, textureAroundSunPtr };
    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 3, textures);
  }

  void SkydomeShader::drawIndexed(const int i_count, const int i_startIndex) const
  {
    getRenderDevice().getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx
