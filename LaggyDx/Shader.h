#pragma once

#include "IShader.h"


namespace Dx
{
  class Shader : public IShader
  {
  public:
    Shader(IRenderDevice& i_renderDevice);

    virtual bool getFillMode() const override;
    virtual void setFillMode(bool i_solid) override;

  protected:
    virtual void setRenderStates() const;
    RenderDevice& getRenderDevice() const;
    
  private:
    bool d_solidFillMode = true;
    RenderDevice& d_renderDevice;
  };

} // ns Dx
