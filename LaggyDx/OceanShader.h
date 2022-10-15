#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class OceanShader
  {
  public:
    OceanShader(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);
    ~OceanShader();

    void setDrawAabb(bool i_draw);

    void draw(const IObject3& i_object) const;

  private:
    RenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;
    const ICamera& d_camera;
    const ITextureResource& d_emptyTexture;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_lightBuffer = nullptr;

    bool d_drawAabb = false;

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
    void setBuffers(const Mesh& i_mesh) const;
    void setMatrices(const IObject3& i_object) const;
    void setTexture(const IObject3& i_object) const;
    void setTexture(const Material& i_material) const;
    void setMaterial(const Material& i_material) const;
    void drawIndexed(int i_count, int i_startIndex) const;
  };

} // ns Dx
