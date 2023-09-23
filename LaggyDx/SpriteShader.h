#pragma once

#include "CBuffer.h"
#include "IMesh.h"
#include "ISpriteShader.h"


namespace Dx
{
  class SpriteShader : public ISpriteShader
  {
  public:
    SpriteShader(const ICamera2* i_camera);

    virtual void draw(
      const ISprite& i_sprite,
      const UvOffset* i_uvOffset = nullptr,
      bool i_disableCameraView = false) const override;

  private:
    const ICamera2* d_camera = nullptr;
    const ITexture& d_emptyTexture;
    std::unique_ptr<IMesh> d_spriteMesh;
    
    CBuffer d_matrixBuffer;
    CBuffer d_uvOffsetBuffer;
    CBuffer d_colorBuffer;

    const UvOffset d_defaultUvOffset;

    DirectX::XMMATRIX d_defaultViewMatrix;
    DirectX::XMMATRIX d_projMatrix;
    void createMatrices();

    void createSpriteMesh();

    void setXfmMatrices(const ISprite& i_sprite, bool i_disableCameraView) const;
    void setTexture(const ISprite& i_sprite) const;
    void setUvOffset(const UvOffset& i_uvOffset) const;
    void setColor(const Sdk::Vector4F& i_color) const;
    void setGeometryBuffers() const;
  };

} // ns Dx
