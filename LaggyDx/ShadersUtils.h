#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  DirectX::XMFLOAT3 getXmfloat3(const Sdk::Vector3F& i_input);

  DirectX::XMFLOAT3 getXmfloat3(const Sdk::Vector3D& i_input);

  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector4F& i_input);

  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector4D& i_input);


  DirectX::XMFLOAT2 getNormalized(Sdk::Vector2D i_input);

  DirectX::XMFLOAT3 getNormalized(Sdk::Vector3D i_input);

} // ns Dx
