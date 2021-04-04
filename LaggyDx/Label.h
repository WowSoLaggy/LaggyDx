#pragma once

#include "Control.h"
#include "Text.h"


namespace Dx
{
  class Label : public Control
  {
  public:
    void setText(std::string i_text);
    const std::string& getText() const;

    void setFont(const std::string& i_fontName);

    virtual void render(IRenderer2d& i_renderer, const Sdk::Vector2F& i_parentPos) const override;

  private:
    Text d_text;
  };

} // Dx
