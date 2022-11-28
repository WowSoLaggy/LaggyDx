#pragma once

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
    virtual ~SkydomeShader() override;

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

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_colorsBuffer = nullptr;
    ID3D11Buffer* d_skyDomeBuffer = nullptr;

    ID3D11SamplerState* d_sampleState = nullptr;

    void createShaders();
    void disposeShaders();

    void createBuffers();
    void disposeBuffers();

    virtual void setRenderStates() const override;
    void setShaders() const;
    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTextures() const;
    void drawIndexed(int i_count, int i_startIndex) const;
  };

} // ns Dx
