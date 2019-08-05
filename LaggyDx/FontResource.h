#pragma once

#include "IFontResource.h"
#include "ILoadableResource.h"


namespace Dx
{
  class FontResource : public ILoadableResource, public IFontResource
  {
  public:

    FontResource(std::string i_fontFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual Sdk::RectI getStringRect(const std::string& i_string) const override;

    std::shared_ptr<SpriteFont> getSpriteFont() const { return d_spriteFont; }

  private:

    const std::string d_fontFilePath;
    std::shared_ptr<SpriteFont> d_spriteFont;

  };

} // ns Dx
