#include "stdafx.h"
#include "SpriteShader.h"

#include "DefaultMeshes.h"
#include "ICamera2.h"
#include "IResourceController.h"
#include "IShape2d.h"
#include "ISprite.h"
#include "ITexture.h"
#include "RenderDevice.h"
#include "ShaderBuffers.h"
#include "VertexLayout.h"

#include "Generated/Sprite.gen.ps.h"
#include "Generated/Sprite.gen.vs.h"


namespace Dx
{
  SpriteShader::SpriteShader(const ICamera2* i_camera)
    : d_camera(i_camera)
    , d_matrixBuffer(getRenderDevice(), sizeof(WorldViewProj))
    , d_uvOffsetBuffer(getRenderDevice(), sizeof(UvOffsetDesc))
    , d_colorBuffer(getRenderDevice(), sizeof(ColorDesc))
    , d_emptyTexture(getResourceController().getTexture("white.png"))
  {
    getShaders().initVs(g_spriteVs, sizeof(g_spriteVs), getVertexLayoutPos2Text());
    getShaders().initPs(g_spritePs, sizeof(g_spritePs));
    getShaders().addSampler(true);

    disableDepthBuffer();
    createSpriteMesh();
    createMatrices();
  }


  void SpriteShader::draw(
    const ISprite& i_sprite,
    const UvOffset* i_uvOffset,
    bool i_disableCameraView) const
  {
    setRenderStates();
    setShaders();
    setXfmMatrices(i_sprite, i_disableCameraView);
    setTexture(i_sprite);
    setUvOffset(i_uvOffset ? *i_uvOffset : d_defaultUvOffset);
    setColor(i_sprite.getColor());
    setGeometryBuffers();
    drawIndexed(d_spriteMesh->getIndexBuffer().getIndexCount(), 0);
  }


  void SpriteShader::createSpriteMesh()
  {
    d_spriteMesh = DefaultMeshes::rectangle(1, 1);
  }

  void SpriteShader::createMatrices()
  {
    d_defaultViewMatrix = XMMatrixTranspose(XMMatrixIdentity());

    const float left = 0;
    const float right = (float)getRenderDevice().getResolution().x;
    const float bottom = (float)getRenderDevice().getResolution().y;
    const float top = 0;
    const float nearZ = 0;
    const float farZ = 1;
    d_projMatrix = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(left, right, bottom, top, nearZ, farZ));
  }


  void SpriteShader::setXfmMatrices(const ISprite& i_sprite, bool i_disableCameraView) const
  {
    auto getWorldMatrixTransposed = [&]()
    {
      const auto& position = i_sprite.getPosition();
      const float rotation = (float)i_sprite.getRotation();
      const float scale = (float)i_sprite.getScale();
      const float xSize = (float)i_sprite.getSize().x;
      const float ySize = (float)i_sprite.getSize().y;
      const auto worldMatrix =
        XMMatrixScaling(scale * xSize, scale * ySize, 1) *
        XMMatrixRotationRollPitchYaw(0, 0, 0) *
        XMMatrixTranslation((float)position.x, (float)position.y, 0);

      return XMMatrixTranspose(worldMatrix);
    };

    auto getViewMatrixTransposed = [&]()
    {
      if (i_disableCameraView || !d_camera)
        return d_defaultViewMatrix;

      const auto& offset = -d_camera->getOffset();
      const auto viewMatrix = XMMatrixTranslation((float)offset.x, (float)offset.y, 0);
      return XMMatrixTranspose(viewMatrix);
    };


    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_matrixBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (WorldViewProj*)mappedResource.pData;
      dataPtr->world = getWorldMatrixTransposed();
      dataPtr->view = getViewMatrixTransposed();
      dataPtr->projection = d_projMatrix;
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_matrixBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(0, 1, d_matrixBuffer.getPp());
  }

  void SpriteShader::setTexture(const ISprite& i_sprite) const
  {
    ID3D11ShaderResourceView* texturePtr = nullptr;

    if (const auto* texture = i_sprite.getTexture())
      texturePtr = texture->getTexturePtr();
    else
      texturePtr = d_emptyTexture.getTexturePtr();

    getRenderDevice().getDeviceContextPtr()->PSSetShaderResources(0, 1, &texturePtr);
  }

  void SpriteShader::setUvOffset(const UvOffset& i_uvOffset) const
  {
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_uvOffsetBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (UvOffsetDesc*)mappedResource.pData;
      dataPtr->offset.x = i_uvOffset.offset.x;
      dataPtr->offset.y = i_uvOffset.offset.y;
      dataPtr->multiplier.x = i_uvOffset.multiplier.x;
      dataPtr->multiplier.y = i_uvOffset.multiplier.y;
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_uvOffsetBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->VSSetConstantBuffers(1, 1, d_uvOffsetBuffer.getPp());
  }

  void SpriteShader::setColor(const Sdk::Vector4F& i_color) const
  {
    {
      D3D11_MAPPED_SUBRESOURCE mappedResource;
      getRenderDevice().getDeviceContextPtr()->Map(d_colorBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

      auto* dataPtr = (ColorDesc*)mappedResource.pData;
      dataPtr->color.x = i_color.x;
      dataPtr->color.y = i_color.y;
      dataPtr->color.z = i_color.z;
      dataPtr->color.w = i_color.w;
    }

    getRenderDevice().getDeviceContextPtr()->Unmap(d_colorBuffer.get(), 0);

    getRenderDevice().getDeviceContextPtr()->PSSetConstantBuffers(0, 1, d_colorBuffer.getPp());
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
