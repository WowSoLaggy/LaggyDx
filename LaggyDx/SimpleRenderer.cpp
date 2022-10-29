#include "stdafx.h"
#include "SimpleRenderer.h"

#include "ICamera.h"
#include "IMaterialSequence.h"
#include "IndexBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "Model.h"
#include "PsResource.h"
#include "RenderDevice.h"
#include "ShaderBuffers.h"
#include "TextureResource.h"
#include "VertexBuffer.h"
#include "VsResource.h"


namespace Dx
{
  SimpleRenderer::SimpleRenderer(
    IRenderDevice& i_renderDevice,
    const ICamera& i_camera,
    const IResourceController& i_resourceController)
    : d_renderDevice(i_renderDevice)
    , d_resourceController(i_resourceController)
    , d_camera(i_camera)
    , d_pixelShader(i_resourceController.getPs("SimpleShader.ps.hlsl"))
    , d_vertexShader(i_resourceController.getVs("SimpleShader.vs.hlsl"))
    , d_emptyTexture(i_resourceController.getTexture("white.png"))
  {
    createBuffers();
  }

  SimpleRenderer::~SimpleRenderer()
  {
    deleteBuffers();
  }


  void SimpleRenderer::setDrawAabb(bool i_draw)
  {
    d_drawAabb = i_draw;
  }


  void SimpleRenderer::draw(const IObject3& i_object) const
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


  void SimpleRenderer::createBuffers()
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);

    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightCBuffer);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    renderDevice.getDevicePtr()->CreateBuffer(&matrixBufferDesc, nullptr, &d_matrixBuffer);
    renderDevice.getDevicePtr()->CreateBuffer(&lightBufferDesc, nullptr, &d_lightBuffer);
  }

  void SimpleRenderer::deleteBuffers()
  {
    d_matrixBuffer->Release();
    d_matrixBuffer = nullptr;

    d_lightBuffer->Release();
    d_lightBuffer = nullptr;
  }


  void SimpleRenderer::setRenderStates() const
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    renderDevice.resetState();
  }

  void SimpleRenderer::setShaders() const
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);

    const auto& vsResource = dynamic_cast<const VsResource&>(d_vertexShader);
    const auto& psResource = dynamic_cast<const PsResource&>(d_pixelShader);
    auto* samplerState = psResource.getSampleStatePtr();

    renderDevice.getDeviceContextPtr()->IASetInputLayout(vsResource.getLayoutPtr());
    renderDevice.getDeviceContextPtr()->VSSetShader(vsResource.getPtr(), nullptr, 0);
    renderDevice.getDeviceContextPtr()->PSSetShader(psResource.getPtr(), nullptr, 0);
    renderDevice.getDeviceContextPtr()->PSSetSamplers(0, 1, &samplerState);
  }

  void SimpleRenderer::setBuffers(const Mesh& i_mesh) const
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);

    auto* vbPtr = i_mesh.getVertexBuffer().getPtr();
    unsigned int stride = i_mesh.getVertexBuffer().getStride();
    auto* ibPtr = i_mesh.getIndexBuffer().getPtr();
    unsigned int offset = 0;

    renderDevice.getDeviceContextPtr()->IASetVertexBuffers(0, 1, &vbPtr, &stride, &offset);
    renderDevice.getDeviceContextPtr()->IASetIndexBuffer(ibPtr, DXGI_FORMAT_R32_UINT, 0);

    const auto topology = i_mesh.getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    renderDevice.getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

  void SimpleRenderer::setMatrices(const IObject3& i_object) const
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);

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
    renderDevice.getDeviceContextPtr()->Map(d_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (MatrixBuffer*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    renderDevice.getDeviceContextPtr()->Unmap(d_matrixBuffer, 0);

    renderDevice.getDeviceContextPtr()->VSSetConstantBuffers(0, 1, &d_matrixBuffer);
  }

  void SimpleRenderer::setTexture(const IObject3& i_object) const
  {
    auto* texturePtr = static_cast<const TextureResource&>(d_emptyTexture).getTexturePtr();

    if (const auto* textureResource = dynamic_cast<const TextureResource*>(i_object.getTextureResource()))
      texturePtr = textureResource->getTexturePtr();

    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);
    renderDevice.getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void SimpleRenderer::setTexture(const Material& i_material) const
  {
    if (!i_material.textureName.empty())
    {
      const auto& texture = dynamic_cast<const TextureResource&>(
        d_resourceController.getTexture(i_material.textureName));
      auto* texturePtr = texture.getTexturePtr();

      const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);
      renderDevice.getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
  }

  void SimpleRenderer::setMaterial(const Material& i_material) const
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    renderDevice.getDeviceContextPtr()->Map(d_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (LightCBuffer*)mappedResource.pData;
    dataPtr->diffuseColor = XMFLOAT4(
      i_material.diffuseColor.x,
      i_material.diffuseColor.y,
      i_material.diffuseColor.z,
      i_material.diffuseColor.w);
    dataPtr->lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    dataPtr->lightDirection = { 1.0f, -1.0f, -1.0f };
    dataPtr->ambientStrength = 0.3f;

    renderDevice.getDeviceContextPtr()->Unmap(d_lightBuffer, 0);

    renderDevice.getDeviceContextPtr()->PSSetConstantBuffers(0, 1, &d_lightBuffer);
  }

  void SimpleRenderer::drawIndexed(const int i_count, const int i_startIndex) const
  {
    const auto& renderDevice = dynamic_cast<const RenderDevice&>(d_renderDevice);
    renderDevice.getDeviceContextPtr()->DrawIndexed(i_count, i_startIndex, 0);
  }

} // ns Dx
