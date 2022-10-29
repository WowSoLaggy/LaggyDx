#include "stdafx.h"
#include "ShadersUtils.h"


namespace Dx
{
  DirectX::XMFLOAT3 getXmfloat3(const Sdk::Vector3F& i_input)
  {
    return { i_input.x, i_input.y, i_input.z };
  }

  DirectX::XMFLOAT3 getXmfloat3(const Sdk::Vector3D& i_input)
  {
    return getXmfloat3(i_input.getVector<float>());
  }

  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector4F& i_input)
  {
    return { i_input.x, i_input.y, i_input.z, i_input.w };
  }

  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector4D& i_input)
  {
    return getXmfloat4(i_input.getVector<float>());
  }


  DirectX::XMFLOAT2 getNormalized(Sdk::Vector2D i_input)
  {
    i_input.normalize();
    return { (float)i_input.x, (float)i_input.y };
  }

  DirectX::XMFLOAT3 getNormalized(Sdk::Vector3D i_input)
  {
    i_input.normalize();
    return getXmfloat3(i_input);
  }

} // anonym NS