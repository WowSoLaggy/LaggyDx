#pragma once

#include "CBuffer.h"
#include "IInstancedDepthShader.h"


namespace Dx
{
  class InstancedDepthShader : public IInstancedDepthShader
  {
  public:
    InstancedDepthShader(const ShadowCamera& i_shadowCamera);

    virtual void draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const override;

  protected:
    virtual void setRenderStates() const override;

  private:
    const ShadowCamera& d_shadowCamera;
    const ITexture& d_emptyTexture;

    CBuffer d_matrixBuffer;

    void setGeometryBuffers(const IMesh& i_mesh, const InstanceBuffer& i_instances) const;
    void setXfmMatrices() const;
    void setTexture(const Material& i_material) const;
  };

} // ns Dx
