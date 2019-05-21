#pragma once

#include "IResource.h"

#include <LaggySdk/Vector.h>


struct ID3D11ShaderResourceView;


namespace Dx
{

  class TextureResource : public IResource
  {
  public:
    TextureResource(std::string i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    const Sdk::Vector2& getSize() const { return d_size; }

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    const std::string d_textureFilePath;

    Sdk::Vector2 d_size;

    ID3D11ShaderResourceView* d_texture;
  };

} // ns Dx
