#pragma once

#include "ISimpleRenderer.h"
#include "LaggyDxFwd.h"


namespace Dx
{

  class SimpleRenderer : public ISimpleRenderer
  {
  public:
    SimpleRenderer(
      const IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);
    ~SimpleRenderer();

    virtual void draw(const IObject3& i_object) override;

  private:
    const IRenderDevice& d_renderDevice;
    const ICamera& d_camera;
    const IPsResource& d_pixelShader;
    const IVsResource& d_vertexShader;

    ID3D11Buffer* d_matrixBuffer = nullptr;
    ID3D11Buffer* d_lightBuffer = nullptr;

    void createBuffers();
    void deleteBuffers();

    void setShaders();
    void setBuffers(const IObject3& i_object);
    void setMatrices(const IObject3& i_object);
    void setTexture(const IObject3& i_object);
    void setMaterial(const Material& i_material);
    void drawIndexed(int i_count, int i_startIndex);
  };

} // ns Dx
