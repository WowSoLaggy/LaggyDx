#pragma once

#include "IOceanShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  class OceanShader : public IOceanShader
  {
  public:
    OceanShader(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);
    virtual ~OceanShader() override;

    virtual void setGlobalTime(double i_time) override;

    virtual void setWaterColor(const Sdk::Vector4D& i_color) override;

    virtual void setWindDirection(int i_waveIndex, Sdk::Vector2D i_direction) override;
    virtual void setWavesSteepness(int i_waveIndex, double i_steepness) override;
    virtual void setWavesLength(int i_waveIndex, double i_length) override;

    virtual void setLightDirection(Sdk::Vector3D i_direction) override;
    virtual void setLightColor(const Sdk::Vector4D& i_color) override;
    virtual void setAmbientStrength(double i_strength) override;

    virtual void setFillMode(bool i_solid) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    TimeCBuffer d_timeCBuffer;
    WaveCBuffer d_waveCBuffer;
    LightCBuffer d_lightCBuffer;

    RenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;
    const ICamera& d_camera;
    const ITextureResource& d_emptyTexture;
    const ITextureResource& d_bumpTexture;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_cameraBuffer = nullptr;
    ID3D11Buffer* d_lightBuffer = nullptr;
    ID3D11Buffer* d_timeBuffer = nullptr;
    ID3D11Buffer* d_waveBuffer = nullptr;

    ID3D11PixelShader* d_pixelShader = nullptr;
    ID3D11SamplerState* d_sampleState = nullptr;
    ID3D11VertexShader* d_vertexShader = nullptr;
    ID3D11InputLayout* d_layout = nullptr;

    bool d_solidFillMode = true;

    void createShaders();
    void disposeShaders();

    void createBuffers();
    void disposeBuffers();

    void setRenderStates() const;
    void setShaders() const;
    void setGeometryBuffers(const IMesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setBumpTexture() const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;
    void drawIndexed(int i_count, int i_startIndex) const;

    XMFLOAT4& getWaveByIndex(int i_waveIndex);
  };

} // ns Dx
