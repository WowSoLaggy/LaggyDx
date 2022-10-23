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
    DirectX::XMFLOAT2 getNormalized(Sdk::Vector2D i_input)
    {
      i_input.normalize();
      return { (float)i_input.x, (float)i_input.y };
    }

    DirectX::XMFLOAT3 getNormalized(Sdk::Vector3D i_input)
    {
      i_input.normalize();
      return { (float)i_input.x, (float)i_input.y, (float)i_input.z };
    }

    DirectX::XMFLOAT4 getXmfloat4(Sdk::Vector4F i_input)
    {
      return { i_input.x, i_input.y, i_input.z, i_input.w };
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


  void OceanShader::setGlobalTime(const double i_time)
  {
    d_globalCBuffer.time = (float)i_time;
  }

  void OceanShader::setTextureCoef(const double i_coef)
  {
    d_globalCBuffer.textureCoef = (float)i_coef;
  }


  void OceanShader::setWindDirection(Sdk::Vector2D i_direction)
  {
    const auto normalizedDirection = getNormalized(i_direction);
    d_waveCBuffer.wave.x = normalizedDirection.x;
    d_waveCBuffer.wave.y = normalizedDirection.y;
  }

  void OceanShader::setWavesSteepness(const double i_steepness)
  {
    d_waveCBuffer.wave.z = (float)i_steepness;
  }

  void OceanShader::setWavesLength(const double i_length)
  {
    d_waveCBuffer.wave.w = (float)i_length;
  }


  void OceanShader::draw(const IObject3& i_object) const
  {
    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setCBuffers();
    setTexture(i_object);

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials().getMaterialSpans())
      {
        setTexture(materialSpan.material);
        setMaterial(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(mesh);
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
    auto createBuffer = [&](const int i_sizeOf, ID3D11Buffer** i_buf)
    {
      D3D11_BUFFER_DESC desc;
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.ByteWidth = i_sizeOf;
      desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      desc.MiscFlags = 0;
      desc.StructureByteStride = 0;

      HRESULT hRes = d_renderDevice.getDevicePtr()->CreateBuffer(&desc, nullptr, i_buf);
      CONTRACT_ASSERT(!FAILED(hRes));
    };

    createBuffer(sizeof(MatrixBuffer), &d_matrixBuffer);
    createBuffer(sizeof(LightingCBuffer), &d_lightBuffer);
    createBuffer(sizeof(GlobalCBuffer), &d_globalBuffer);
    createBuffer(sizeof(WaveCBuffer), &d_waveBuffer);
  }

  void OceanShader::disposeBuffers()
  {
    d_waveBuffer->Release();
    d_waveBuffer = nullptr;

    d_globalBuffer->Release();
    d_globalBuffer = nullptr;

    d_lightBuffer->Release();
    d_lightBuffer = nullptr;

    d_matrixBuffer->Release();
    d_matrixBuffer = nullptr;
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

  void OceanShader::setGeometryBuffers(const Mesh& i_mesh) const
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
    d_renderDevice.getDeviceContextPtr()->Map(d_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (MatrixBuffer*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    d_renderDevice.getDeviceContextPtr()->Unmap(d_matrixBuffer, 0);

    d_renderDevice.getDeviceContextPtr()->VSSetConstantBuffers(0, 1, &d_matrixBuffer);
  }

  void OceanShader::setCBuffers() const
  {
    // Global
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      d_renderDevice.getDeviceContextPtr()->Map(d_globalBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (GlobalCBuffer*)mappedResource.pData;
      *dataPtr = d_globalCBuffer;

      d_renderDevice.getDeviceContextPtr()->Unmap(d_globalBuffer, 0);
      d_renderDevice.getDeviceContextPtr()->VSSetConstantBuffers(1, 1, &d_globalBuffer);
    }

    // Wind
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      d_renderDevice.getDeviceContextPtr()->Map(d_waveBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (WaveCBuffer*)mappedResource.pData;
      *dataPtr = d_waveCBuffer;

      d_renderDevice.getDeviceContextPtr()->Unmap(d_waveBuffer, 0);
      d_renderDevice.getDeviceContextPtr()->VSSetConstantBuffers(2, 1, &d_waveBuffer);
    }
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

    auto* dataPtr = (LightingCBuffer*)mappedResource.pData;
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
