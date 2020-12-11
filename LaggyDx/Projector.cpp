#include "stdafx.h"
#include "Projector.h"


namespace Dx
{
  Sdk::Vector2D Projector::screenToWorld(const Sdk::Vector2I& i_screenCoords)
  {
    if (!d_matrixInverse)
      updateMatrices();
    CONTRACT_ASSERT(d_matrixInverse.get());

    const auto res = XMVector2Transform({ (float)i_screenCoords.x, (float)i_screenCoords.y }, *d_matrixInverse);
    return { XMVectorGetX(res), XMVectorGetY(res) };
  }

  Sdk::Vector2I Projector::worldToScreen(const Sdk::Vector2D& i_worldCoords)
  {
    if (!d_matrixDirect)
      updateMatrices();
    CONTRACT_ASSERT(d_matrixDirect.get());

    const auto res = XMVector2Transform({ (float)i_worldCoords.x, (float)i_worldCoords.y }, *d_matrixDirect);
    return { (int)XMVectorGetX(res), (int)XMVectorGetY(res) };
  }


  void Projector::setTranslation(Sdk::Vector2I i_translation)
  {
    d_translation = std::move(i_translation);
    resetMatrices();
  }

  void Projector::setScaleOrigin(Sdk::Vector2I i_scaleOrigin)
  {
    d_scaleOrigin = std::move(i_scaleOrigin);
    resetMatrices();
  }

  void Projector::setScale(double i_scale)
  {
    d_scale = i_scale;
    resetMatrices();
  }


  void Projector::resetMatrices()
  {
    d_matrixDirect.reset();
    d_matrixInverse.reset();
  }

  void Projector::updateMatrices()
  {
    const auto matrixDirect = XMMatrixTransformation2D(
      { (float)(d_scaleOrigin.x), (float)(d_scaleOrigin.y) }, // scaling origin
      0,
      { (float)d_scale, (float)d_scale },
      { 0, 0 },
      0,
      { (float)d_translation.x, (float)d_translation.y });
    const auto matrixInverse = XMMatrixInverse(nullptr, matrixDirect);

    d_matrixDirect = std::make_shared<DirectX::XMMATRIX>(std::move(matrixDirect));
    d_matrixInverse = std::make_shared<DirectX::XMMATRIX>(std::move(matrixInverse));
  }

} // ns Dx
