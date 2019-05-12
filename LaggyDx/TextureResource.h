#pragma once

#include "IResource.h"


namespace Dx
{
  class TextureResource : public IResource
  {
  public:

    TextureResource(std::string i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:

    const std::string d_textureFilePath;

    ID3D11ShaderResourceView* d_texture;

  };

} // ns Dx
