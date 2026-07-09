#pragma once

#include "CBuffer.h"
#include "IInstancedShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class InstancedShader : public IInstancedShader
  {
  public:
    InstancedShader(const ICamera3& i_camera);

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void setShadowMap(const ITexture& i_shadowMap) override;
    virtual void setShadowCamera(const ShadowCamera& i_shadowCamera) override;

    virtual void draw(const IObject3& i_object, const IInstanceBuffer& i_instances) const override;

  private:
    const ICamera3& d_camera;
    const ITexture& d_emptyTexture;

    // Shadow map at t1; defaults to white.png so receivers stay lit until one is set
    const ITexture* d_shadowMapTexture;
    const ShadowCamera* d_shadowCamera = nullptr;

    LightDesc d_lightDesc;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;
    CBuffer d_shadowMatrixBuffer;

    void setGeometryBuffers(const IMesh& i_mesh, const InstanceBuffer& i_instances) const;
    void setXfmMatrices() const;
    void setCBuffers() const;
    void setShadowCBuffer() const;
    void setShadowTexture() const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx
