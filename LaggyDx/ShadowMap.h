#pragma once

#include "DxResourceWrapper.h"
#include "IShadowMap.h"


namespace Dx
{
  class ShadowMap : public IShadowMap
  {
  public:
    ShadowMap(IRenderDevice& i_renderDevice, int i_resolution);

    virtual int getResolution() const override;
    virtual const ITexture& getTexture() const override;

    virtual void bind() override;
    virtual void unbind() override;

  private:
    RenderDevice& d_renderDevice;
    int d_resolution;

    DxResourceWrapper<ID3D11Texture2D> d_texture2D;
    DxResourceWrapper<ID3D11DepthStencilView> d_depthStencilView;
    DxResourceWrapper<ID3D11ShaderResourceView> d_shaderResourceView;

    std::shared_ptr<ITexture> d_texture;

    void createResources();
  };

} // ns Dx
