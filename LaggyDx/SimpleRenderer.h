#pragma once

#include "ISimpleRenderer.h"
#include "LaggyDxFwd.h"


namespace Dx
{

  class SimpleRenderer : public ISimpleRenderer
  {
  public:
    SimpleRenderer(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);
    ~SimpleRenderer();

    virtual void setDrawAabb(bool i_draw) override;

    virtual void draw(const IObject3& i_object) const override;

  private:
    IRenderDevice& d_renderDevice;
    const IResourceController& d_resourceController;
    const ICamera& d_camera;
    const IPsResource& d_pixelShader;
    const IVsResource& d_vertexShader;
    const ITextureResource& d_emptyTexture;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_lightBuffer = nullptr;

    bool d_drawAabb = false;

    void createBuffers();
    void deleteBuffers();

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
