#pragma once

#include "LaggyDxFwd.h"
#include "IShader.h"
#include "ShaderWrapper.h"


namespace Dx
{
  class Shader : public IShader
  {
  public:
    Shader();

    virtual bool getFillMode() const override;
    virtual void setFillMode(bool i_solid) override;

  protected:
    virtual void setRenderStates() const;
    RenderDevice& getRenderDevice() const;
    IResourceController& getResourceController() const;

    ShaderWrapper& getShaders();
    const ShaderWrapper& getShaders() const;

    void setShaders() const;

    void resetVsResources(int i_numSlots) const;
    void resetPsResources(int i_numSlots) const;
    
    void drawIndexed(int i_count, int i_startIndex) const;

  private:
    bool d_solidFillMode = true;
    ShaderWrapper d_shaders;
  };

} // ns Dx
