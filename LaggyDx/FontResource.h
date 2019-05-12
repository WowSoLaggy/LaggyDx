#pragma once

#include "IResource.h"


class FontResource : public IResource
{
public:

  FontResource(std::string i_fontFilePath);

  virtual void load(IRenderDevice& i_renderDevice) override;
  virtual void unload() override;

  std::shared_ptr<SpriteFont> getSpriteFont() const { return d_spriteFont; }

private:

  const std::string d_fontFilePath;
  std::shared_ptr<SpriteFont> d_spriteFont;

};
