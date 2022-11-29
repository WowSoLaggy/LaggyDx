#pragma once

#include "CBuffer.h"
#include "ISkydomeShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  struct SkydomeSettings
  {
    XMFLOAT3 cameraPosition{ 0, 0, 0 };
    float sunRadiusInternal{ 0 };
    XMFLOAT3 sunDirection{ 0, 0, 0 };
    float sunRadiusExternal{ 0 };
  };


  class SkydomeShader : public ISkydomeShader
  {
  public:
    SkydomeShader(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

    virtual void setSunDirection(Sdk::Vector3D i_sunDir) override;
    virtual void setSunRadiusInternal(float i_radius) override;
    virtual void setSunRadiusExternal(float i_radius) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    const IResourceController& d_resourceController;
    const ICamera& d_camera;

    const ITextureResource& d_mainTexture;
    const ITextureResource& d_horizonHazeTexture;
    const ITextureResource& d_aroundSunTexture;

    SkydomeSettings d_skyDomeSettings;

    CBuffer d_matrixBuffer;
    CBuffer d_skyDomeBuffer;

    virtual void setRenderStates() const override;
    void setShaders() const;
    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTextures() const;
    void drawIndexed(int i_count, int i_startIndex) const;
  };

} // ns Dx
