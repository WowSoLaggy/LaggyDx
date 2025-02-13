#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class TextRendererGuard
  {
  public:
    TextRendererGuard(ITextRenderer& i_textRenderer);
    ~TextRendererGuard();

  private:
    ITextRenderer& d_textRenderer;
  };
} // ns Dx
