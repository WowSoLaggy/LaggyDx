#include "stdafx.h"
#include "InstancedDepthShader.h"

#include "IndexBuffer.h"
#include "InstanceBuffer.h"
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
#include "Generated/InstancedDepth.gen.vs.h"


namespace
{
  // Rasterizer depth bias tuning knobs: raise on shadow acne, lower on peter-panning
  constexpr int c_shadowDepthBias = 16000;
  constexpr float c_shadowSlopeBias = 2.0f;
}


namespace Dx
{
  InstancedDepthShader::InstancedDepthShader(const ShadowCamera& i_shadowCamera)
    : d_shadowCamera(i_shadowCamera)
    , d_emptyTexture(getResourceController().getTexture("white.png"))
    , d_matrixBuffer(getRenderDevice(), sizeof(ViewProj))
  {
    getShaders().initVs(g_instancedDepthVs, sizeof(g_instancedDepthVs), getVertexLayoutPos3NormTextInstanced());
    getShaders().initPs(g_depthPs, sizeof(g_depthPs));
    getShaders().addSampler(true);
  }


  void InstancedDepthShader::draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const
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

    auto drawMesh = [&](const auto& i_mesh)
    {
      setGeometryBuffers(i_mesh, instances);

      for (const auto& materialSpan : i_mesh.getMaterials())
      {
        setTexture(materialSpan.material);
        drawIndexedInstanced(materialSpan.count, materialSpan.startIndex, instances.getInstanceCount());
      }
    };

    for (const auto& mesh : i_object.getModel().getMeshes())
      drawMesh(*mesh);
  }


  void InstancedDepthShader::setRenderStates() const
  {
    ShaderBase::setRenderStates();

    // Push casters slightly away from the light to avoid shadow acne
    auto rasterizerState = getRenderDevice().getRasterizerState();
    rasterizerState.DepthBias = c_shadowDepthBias;
    rasterizerState.SlopeScaledDepthBias = c_shadowSlopeBias;
    rasterizerState.MultisampleEnable = FALSE;
    getRenderDevice().setRasterizerState(std::move(rasterizerState));
  }


  void InstancedDepthShader::setGeometryBuffers(const IMesh& i_mesh, const InstanceBuffer& i_instances) const
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

  void InstancedDepthShader::setXfmMatrices() const
  {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (ViewProj*)mappedResource.pData;
    dataPtr->view = XMMatrixTranspose(d_shadowCamera.getViewMatrix());
    dataPtr->projection = XMMatrixTranspose(d_shadowCamera.getProjectionMatrix());

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void InstancedDepthShader::setTexture(const Material& i_material) const
  {
    auto* texturePtr = d_emptyTexture.getTexturePtr();

    if (!i_material.textureName.empty())
      texturePtr = getResourceController().getTexture(i_material.textureName).getTexturePtr();
    else if (i_material.texture)
      texturePtr = i_material.texture->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

} // ns Dx
