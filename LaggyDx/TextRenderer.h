#pragma once

#include "ITextRenderer.h"


namespace Dx
{
  class TextRenderer : public ITextRenderer
  {
  public:
    TextRenderer();

    virtual void begin() override;
    virtual void end() override;

    virtual void render(
      const std::string& i_text,
      const IFontResource& i_fontResource,
      const Sdk::Vector2F& i_translation,
      const Sdk::Vector4F& i_color,
      float i_scale) const override;

  private:
    std::unique_ptr<CommonStates> d_states;
    std::unique_ptr<SpriteBatch> d_spriteBatch;
  };

} // ns Dx
