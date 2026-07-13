#pragma once

#include "CBuffer.h"
#include "ISimpleShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class SimpleShader : public ISimpleShader
  {
  public:
    SimpleShader(const ICamera3& i_camera);

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void setShadowCascade(int i_cascade, const ShadowCamera& i_camera, const ITexture& i_map) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;
    const ITexture& d_emptyTexture;

    // Cascade maps at t1..t3; default to white.png so receivers stay lit until wired
    const ITexture* d_shadowMapTextures[c_shadowCascadesCount];
    const ShadowCamera* d_shadowCameras[c_shadowCascadesCount] = {};

    LightDesc d_lightDesc;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;
    CBuffer d_shadowMatrixBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setShadowCBuffer() const;
    void setShadowTexture() const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx
