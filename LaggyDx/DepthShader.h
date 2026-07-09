#pragma once

#include "CBuffer.h"
#include "IDepthShader.h"


namespace Dx
{
  class DepthShader : public IDepthShader
  {
  public:
    DepthShader(const ShadowCamera& i_shadowCamera);

    virtual void draw(const IObject3& i_object) const override;

  protected:
    virtual void setRenderStates() const override;

  private:
    const ShadowCamera& d_shadowCamera;
    const ITexture& d_emptyTexture;

    CBuffer d_matrixBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
  };

} // ns Dx
