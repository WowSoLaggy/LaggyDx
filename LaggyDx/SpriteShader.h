#pragma once

#include "CBuffer.h"
#include "IMesh.h"
#include "ISpriteShader.h"


namespace Dx
{
  class SpriteShader : public ISpriteShader
  {
  public:
    SpriteShader();

    virtual void draw(const ISprite& i_sprite) const override;

  private:
    const ITexture& d_emptyTexture;
    std::unique_ptr<IMesh> d_spriteMesh;
    
    CBuffer d_matrixBuffer;

    DirectX::XMMATRIX d_viewMatrix;
    DirectX::XMMATRIX d_projMatrix;
    void createMatrices();

    void createSpriteMesh();

    void setXfmMatrices(const ISprite& i_sprite) const;
    void setTexture(const ISprite& i_sprite) const;
    void setGeometryBuffers() const;
  };

} // ns Dx
