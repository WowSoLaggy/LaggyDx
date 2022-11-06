#pragma once

#include "ISkyboxShader.h"
#include "ShaderBuffers.h"


namespace Dx
{
  struct SkyboxColorsCbuffer
  {
    XMFLOAT4 colorZeroLevel{ 0, 0, 0 ,0 };
    XMFLOAT4 colorTopLevel{ 0, 0, 0, 0 };
  };


  class SkyboxShader : public ISkyboxShader
  {
  public:
    SkyboxShader(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);
    virtual ~SkyboxShader() override;

    virtual void setZeroLevelColor(const Sdk::Vector4F& i_color) override;
    virtual void setTopLevelColor(const Sdk::Vector4F& i_color) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    RenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;
    const ICamera& d_camera;
    const ITextureResource& d_emptyTexture;

    SkyboxColorsCbuffer d_colorsCBuffer;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_colorsBuffer = nullptr;

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
