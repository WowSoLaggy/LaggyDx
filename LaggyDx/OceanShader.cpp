#include "stdafx.h"
#include "OceanShader.h"

#include "ICamera.h"
#include "IMaterialSequence.h"
#include "IndexBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShaderBuffers.h"
#include "TextureResource.h"
#include "VertexBuffer.h"

#include "Generated/Ocean.gen.ps.h"
#include "Generated/Ocean.gen.vs.h"


namespace Dx
{
  namespace
  {
    struct LightDescBuffer
    {
      XMFLOAT4 diffuseColor;
      XMFLOAT4 lightColor;
      XMFLOAT3 lightDirection;
      float ambientStrength;
    };

    DirectX::XMFLOAT3 getNormalized(Sdk::Vector3F i_input)
    {
      i_input.normalize();
      return { i_input.x, i_input.y, i_input.z };
    }

  } // anonym NS


  OceanShader::OceanShader(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
    : d_renderDevice(dynamic_cast<RenderDevice&>(i_renderDevice))
    , d_resourceController(i_resourceController)
    , d_camera(i_camera)
    , d_emptyTexture(i_resourceController.getTexture("white.png"))
  {
    createShaders();
    createBuffers();
  }

  OceanShader::~OceanShader()
  {
    disposeBuffers();
    disposeShaders();
  }


  void OceanShader::setDrawAabb(bool i_draw)
  {
    d_drawAabb = i_draw;
  }


  void OceanShader::draw(const IObject3& i_object) const
  {
    setRenderStates();
    setShaders();
    setMatrices(i_object);
    setTexture(i_object);

    auto drawMesh = [&](const auto& i_mesh)
    {
      setBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials().getMaterialSpans())
      {
        setTexture(materialSpan.material);
        setMaterial(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(mesh);

    if (d_drawAabb)
      drawMesh(i_object.getModel().getAabbMesh());
  }


  void OceanShader::createShaders()
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);

    // PS

    renderDevice.getDevicePtr()->CreatePixelShader(g_oceanPs, sizeof(g_oceanPs), NULL, &d_pixelShader);
    CONTRACT_ASSERT(d_pixelShader != nullptr);

    // Sampler state

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

    renderDevice.getDevicePtr()->CreateSamplerState(&samplerDesc, &d_sampleState);
    CONTRACT_ASSERT(d_sampleState != nullptr);

    // VS

    renderDevice.getDevicePtr()->CreateVertexShader(g_oceanVs, sizeof(g_oceanVs), NULL, &d_vertexShader);
    CONTRACT_ASSERT(d_vertexShader != nullptr);

    // Input layout

    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    renderDevice.getDevicePtr()->CreateInputLayout(polygonLayout, numElements,
      g_oceanVs, sizeof(g_oceanVs), &d_layout);
    CONTRACT_ASSERT(d_layout != nullptr);
  }

  void OceanShader::disposeShaders()
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


  void OceanShader::createBuffers()
  {
    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightDescBuffer);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    HRESULT hRes = d_renderDevice.getDevicePtr()->CreateBuffer(&matrixBufferDesc, nullptr, &d_matrixBuffer);
    CONTRACT_ASSERT(!FAILED(hRes));

    hRes = d_renderDevice.getDevicePtr()->CreateBuffer(&lightBufferDesc, nullptr, &d_lightBuffer);
    CONTRACT_ASSERT(!FAILED(hRes));
  }

  void OceanShader::disposeBuffers()
  {
    d_matrixBuffer->Release();
    d_matrixBuffer = nullptr;

    d_lightBuffer->Release();
    d_lightBuffer = nullptr;
  }


  void OceanShader::setRenderStates() const
  {
    d_renderDevice.resetState();
  }

  void OceanShader::setShaders() const
  {
    d_renderDevice.getDeviceContextPtr()->IASetInputLayout(d_layout);
    d_renderDevice.getDeviceContextPtr()->VSSetShader(d_vertexShader, nullptr, 0);
    d_renderDevice.getDeviceContextPtr()->PSSetShader(d_pixelShader, nullptr, 0);
    d_renderDevice.getDeviceContextPtr()->PSSetSamplers(0, 1, &d_sampleState);
  }

  void OceanShader::setBuffers(const Mesh& i_mesh) const
  {
    auto* vbPtr = i_mesh.getVertexBuffer().getPtr();
    unsigned int stride = i_mesh.getVertexBuffer().getStride();
    auto* ibPtr = i_mesh.getIndexBuffer().getPtr();
    unsigned int offset = 0;

    d_renderDevice.getDeviceContextPtr()->IASetVertexBuffers(0, 1, &vbPtr, &stride, &offset);
    d_renderDevice.getDeviceContextPtr()->IASetIndexBuffer(ibPtr, DXGI_FORMAT_R32_UINT, 0);

    const auto topology = i_mesh.getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    d_renderDevice.getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

  void OceanShader::setMatrices(const IObject3& i_object) const
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
    d_renderDevice.getDeviceContextPtr()->Map(d_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (MatrixBuffer*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    d_renderDevice.getDeviceContextPtr()->Unmap(d_matrixBuffer, 0);

    d_renderDevice.getDeviceContextPtr()->VSSetConstantBuffers(0, 1, &d_matrixBuffer);
  }

  void OceanShader::setTexture(const IObject3& i_object) const
  {
    auto* texturePtr = static_cast<const TextureResource&>(d_emptyTexture).getTexturePtr();

    if (const auto* textureResource = dynamic_cast<const TextureResource*>(i_object.getTextureResource()))
      texturePtr = textureResource->getTexturePtr();

    d_renderDevice.getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void OceanShader::setTexture(const Material& i_material) const
  {
    if (!i_material.textureName.empty())
    {
      const auto& texture = dynamic_cast<const TextureResource&>(
        d_resourceController.getTexture(i_material.textureName));
      auto* texturePtr = texture.getTexturePtr();

      d_renderDevice.getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
  }

  void OceanShader::setMaterial(const Material& i_material) const
  {
    static const auto LightDirection = getNormalized({ 0.0f, -1.0f, 0.0f });

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    d_renderDevice.getDeviceContextPtr()->Map(d_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (LightDescBuffer*)mappedResource.pData;
    dataPtr->diffuseColor = XMFLOAT4(
      i_material.diffuseColor.x,
      i_material.diffuseColor.y,
      i_material.diffuseColor.z,
      i_material.diffuseColor.w);
    dataPtr->lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    dataPtr->lightDirection = LightDirection;
    dataPtr->ambientStrength = 0.3f;

    d_renderDevice.getDeviceContextPtr()->Unmap(d_lightBuffer, 0);

    d_renderDevice.getDeviceContextPtr()->PSSetConstantBuffers(0, 1, &d_lightBuffer);
  }

  void OceanShader::drawIndexed(const int i_count, const int i_startIndex) const
  {
    d_renderDevice.getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx
