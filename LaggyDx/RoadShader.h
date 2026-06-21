#pragma once

#include "CBuffer.h"
#include "IRoadShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class RoadShader : public IRoadShader
  {
  public:
    RoadShader(const ICamera3& i_camera);

    virtual void setRoadTexture(const std::string& i_textureName) override;

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const ICamera3& d_camera;

    // The default road surface, bound to t0 unless the object carries its own
    // texture (e.g. a crossroad patch). Set externally via setRoadTexture.
    const ITexture* d_roadTexture = nullptr;

    LightDesc d_lightDesc;

    CBuffer d_matrixBuffer;
    CBuffer d_cameraBuffer;
    CBuffer d_lightBuffer;

    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTextures(const IObject3& i_object) const;
    void setMaterial(const Material& i_material) const;
  };

} // ns Dx
