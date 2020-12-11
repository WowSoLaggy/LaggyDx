#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class Projector
  {
  public:
    Sdk::Vector2D screenToWorld(const Sdk::Vector2I& i_screenCoords);
    Sdk::Vector2I worldToScreen(const Sdk::Vector2D& i_worldCoords);

    void setTranslation(Sdk::Vector2I i_translation);
    void setScaleOrigin(Sdk::Vector2I i_scaleOrigin);
    void setScale(double i_scale);

  private:
    Sdk::Vector2I d_translation;
    Sdk::Vector2I d_scaleOrigin;
    double d_scale = 1;

    std::shared_ptr<DirectX::XMMATRIX> d_matrixDirect;
    std::shared_ptr<DirectX::XMMATRIX> d_matrixInverse;

    void resetMatrices();
    void updateMatrices();
  };

} // ns Dx
