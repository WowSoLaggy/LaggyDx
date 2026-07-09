#include "stdafx.h"
#include "DepthShader.h"

#include "IndexBuffer.h"
#include "IObject3.h"
#include "IResourceController.h"
#include "MemoryTexture.h"
#include "Model.h"
#include "RenderDevice.h"
#include "ShaderBuffers.h"
#include "ShadowCamera.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

#include "Generated/Depth.gen.ps.h"
#include "Generated/Depth.gen.vs.h"


namespace
{
  // Rasterizer depth bias tuning knobs: raise on shadow acne, lower on peter-panning
  constexpr int c_shadowDepthBias = 16000;
  constexpr float c_shadowSlopeBias = 2.0f;
}


namespace Dx
{
  DepthShader::DepthShader(const ShadowCamera& i_shadowCamera)
    : d_shadowCamera(i_shadowCamera)
    , d_emptyTexture(getResourceController().getTexture("white.png"))
    , d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
  {
    getShaders().initVs(g_depthVs, sizeof(g_depthVs), getVertexLayoutPos3NormText());
    getShaders().initPs(g_depthPs, sizeof(g_depthPs));
    getShaders().addSampler(true);
  }


  void DepthShader::draw(const IObject3& i_object) const
  {
    if (!isEnabled())
      return;

    if (!i_object.getVisible())
      return;

    setRenderStates();
    setShaders();
    setXfmMatrices(i_object);
    setTexture(i_object);

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setTexture(materialSpan.material);
        drawIndexed(materialSpan.count, materialSpan.startIndex);
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void DepthShader::setRenderStates() const
  {
    ShaderBase::setRenderStates();

    // Push casters slightly away from the light to avoid shadow acne
    auto rasterizerState = getRenderDevice().getRasterizerState();
    rasterizerState.DepthBias = c_shadowDepthBias;
    rasterizerState.SlopeScaledDepthBias = c_shadowSlopeBias;
    rasterizerState.MultisampleEnable = FALSE;
    getRenderDevice().setRasterizerState(std::move(rasterizerState));
  }


  void DepthShader::setGeometryBuffers(const IMesh& i_mesh) const
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

  void DepthShader::setXfmMatrices(const IObject3& i_object) const
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
    const auto viewMatrix = XMMatrixTranspose(d_shadowCamera.getViewMatrix());
    const auto projectionMatrix = XMMatrixTranspose(d_shadowCamera.getProjectionMatrix());

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (WorldViewProj*)mappedResource.pData;
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void DepthShader::setTexture(const IObject3& i_object) const
  {
    auto* texturePtr = d_emptyTexture.getTexturePtr();

    if (const auto* texture = i_object.getTexture())
      texturePtr = texture->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void DepthShader::setTexture(const Material& i_material) const
  {
    if (!i_material.textureName.empty())
    {
      const auto& texture = getResourceController().getTexture(i_material.textureName);
      auto* texturePtr = texture.getTexturePtr();

      getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
    else if (i_material.texture)
    {
      auto* texturePtr = i_material.texture->getTexturePtr();
      getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
    }
  }

} // ns Dx
