#pragma once

#include "ILoadableResource.h"
#include "ITextureResource.h"

#include <LaggySdk/Vector.h>


struct ID3D11ShaderResourceView;


namespace Dx
{
  class TextureResource : public ILoadableResource, public ITextureResource
  {
  public:
    TextureResource(std::string i_textureFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    const Sdk::Vector2& getSize() const override { return d_size; }

    ID3D11ShaderResourceView* getTexturePtr() const { return d_texture; }

  private:
    bool d_loaded = false;
    const std::string d_textureFilePath = "";
    Sdk::Vector2 d_size{ 0, 0 };

    ID3D11ShaderResourceView* d_texture = nullptr;
  };

} // ns Dx
