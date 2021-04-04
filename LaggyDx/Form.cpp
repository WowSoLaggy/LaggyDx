#include "stdafx.h"
#include "Form.h"


namespace Dx
{
  void Form::render(IRenderer2d& i_renderer) const
  {
    Control::render(i_renderer, { 0.0f, 0.0f });
  }

} // Dx
