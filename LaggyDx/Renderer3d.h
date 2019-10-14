#pragma once

#include "IRenderer3d.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class Renderer3d : public IRenderer3d
  {
  public:

    Renderer3d(
      IRenderDevice& io_renderDevice,
      IResourceController& i_resourceController,
      const ICamera& i_camera);
    ~Renderer3d();

    virtual void beginScene() override;
    virtual void endScene() override;

    virtual void renderObject(
      const ITextureResource& i_textureResource,
      const VertexBuffer& i_vertexBuffer, const IndexBuffer& i_indexBuffer,
      const std::vector<MaterialSpan>& i_materialSpans,
      const Sdk::Vector3F& i_position, const Sdk::Vector3F& i_rotation) override;

    virtual void renderObject(
      const IMeshResourceCmo& i_meshCmoResource, const ITextureResource* i_textureResource,
      std::shared_ptr<IAnimationController3d> i_animationController,
      const Sdk::Vector3F& i_position, const Sdk::Vector3F& i_rotation, const Sdk::Vector3F& i_scale,
      bool i_useLighting) override;

  private:

    IRenderDevice& d_renderDevice;
    IResourceController& d_resourceController;
    const ICamera& d_camera;

    std::shared_ptr<CommonStates> d_commonStates;

    ID3D11Buffer* d_matrixBuffer;
    ID3D11Buffer* d_lightBuffer;

    void createBuffers();
    void disposeBuffers();

    void setBuffers(ID3D11Buffer* i_vertexBufferPtr, ID3D11Buffer* i_indexBufferPtr, unsigned int i_stride);
    void setShaders();
    void setShaderMatrices(const Sdk::Vector3F& i_position, const Sdk::Vector3F& i_rotation);
    void setShaderTexture(ID3D11ShaderResourceView* i_texture);
    void setShaderMaterial(const Material& i_material);
    void drawMaterial(const MaterialSpan& i_materialSpan);

  };

} // ns Dx
