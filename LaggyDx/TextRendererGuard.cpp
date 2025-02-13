#include "stdafx.h"
#include "TextRendererGuard.h"

#include "ITextRenderer.h"


namespace Dx
{
  TextRendererGuard::TextRendererGuard(ITextRenderer& i_textRenderer)
    : d_textRenderer(i_textRenderer)
  {
    d_textRenderer.beginScene();
  }

  TextRendererGuard::~TextRendererGuard()
  {
    d_textRenderer.endScene();
  }

} // ns Dx
