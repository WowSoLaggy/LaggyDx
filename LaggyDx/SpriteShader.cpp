#include "stdafx.h"
#include "SpriteShader.h"

#include "IResourceController.h"
#include "IShape2d.h"
#include "ISprite.h"
#include "ITexture.h"
#include "ModelUtils.h"
#include "RenderDevice.h"
#include "ShaderBuffers.h"

#include "Generated/Sprite.gen.ps.h"
#include "Generated/Sprite.gen.vs.h"


namespace Dx
{
  SpriteShader::SpriteShader()
    : d_matrixBuffer(getRenderDevice(), sizeof(WorldMatrix))
    , d_emptyTexture(getResourceController().getTexture("white.png"))
  {
    getShaders().initVs(g_spriteVs, sizeof(g_spriteVs));
    getShaders().initPs(g_spritePs, sizeof(g_spritePs));
    getShaders().addSampler(true);

    createSpriteMesh();
  }


  void SpriteShader::draw(const ISprite& i_sprite) const
  {
    setRenderStates();
    setShaders();
    setXfmMatrices(i_sprite);
    setTexture(i_sprite);
    setGeometryBuffers();
    drawIndexed(4, 0);
  }


  void SpriteShader::createSpriteMesh()
  {
    const auto shape = IShape2d::createRect(10, 10);
    d_spriteMesh = createMeshFromShape(*shape, getRenderDevice());
  }


  void SpriteShader::setXfmMatrices(const ISprite& i_sprite) const
  {
    auto getWorldMatrixTransposed = [&]()
    {
      const auto& position = i_sprite.getPosition();
      const float rotation = (float)i_sprite.getRotation();
      const float scale = (float)i_sprite.getScale();
      const auto worldMatrix =
        XMMatrixScaling(scale, scale, scale) *
        XMMatrixRotationRollPitchYaw(0, 0, 0) *
        XMMatrixTranslation((float)position.x, (float)position.y, 0);

      return XMMatrixTranspose(worldMatrix);
    };

    const auto worldMatrix = getWorldMatrixTransposed();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    auto* dataPtr = (WorldMatrix*)mappedResource.pData;
    dataPtr->world = worldMatrix;

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void SpriteShader::setTexture(const ISprite& i_sprite) const
  {
    auto* texturePtr = d_emptyTexture.getTexturePtr();

    if (const auto* texture = i_sprite.getTexture())
      texturePtr = texture->getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void SpriteShader::setGeometryBuffers() const
  {
    CONTRACT_EXPECT(d_spriteMesh);

    auto* vbPtr = d_spriteMesh->getVertexBuffer().getPtr();
    unsigned int stride = d_spriteMesh->getVertexBuffer().getStride();
    auto* ibPtr = d_spriteMesh->getIndexBuffer().getPtr();
    unsigned int offset = 0;

    getRenderDevice().getDeviceContextPtr()->IASetVertexBuffers(0, 1, &vbPtr, &stride, &offset);
    getRenderDevice().getDeviceContextPtr()->IASetIndexBuffer(ibPtr, DXGI_FORMAT_R32_UINT, 0);

    const auto topology = d_spriteMesh->getTopology() == Topology::TriangleList
      ? D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
      : D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;

    getRenderDevice().getDeviceContextPtr()->IASetPrimitiveTopology(topology);
  }

} // ns Dx
