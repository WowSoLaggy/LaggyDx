#pragma once

#include "IOceanShader.h"
#include "OceanShader_buffers.h"


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
    virtual void setTextureCoef(double i_coef) override;
    virtual void setWindDirection(Sdk::Vector2D i_direction) override;
    virtual void setWindSpeed(double i_speed) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    GlobalCBuffer d_globalCBuffer;
    WindCBuffer d_windCBuffer;

    RenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;
    const ICamera& d_camera;
    const ITextureResource& d_emptyTexture;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_lightBuffer = nullptr;
    ID3D11Buffer* d_globalBuffer = nullptr;
    ID3D11Buffer* d_windBuffer = nullptr;

    ID3D11PixelShader* d_pixelShader = nullptr;
    ID3D11SamplerState* d_sampleState = nullptr;
    ID3D11VertexShader* d_vertexShader = nullptr;
    ID3D11InputLayout* d_layout = nullptr;

    void createShaders();
    void disposeShaders();

    void createBuffers();
    void disposeBuffers();

    void setRenderStates() const;
    void setShaders() const;
    void setGeometryBuffers(const Mesh& i_mesh) const;
    void setXfmMatrices(const IObject3& i_object) const;
    void setCBuffers() const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;
    void drawIndexed(int i_count, int i_startIndex) const;
  };

} // ns Dx
