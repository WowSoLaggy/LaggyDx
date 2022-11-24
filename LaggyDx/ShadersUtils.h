#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  template <typename T>
  DirectX::XMFLOAT2 getXmfloat2(const Sdk::Vector2<T>& i_input)
  {
    return { (float)i_input.x, (float)i_input.y };
  }

  template <typename T>
  DirectX::XMFLOAT3 getXmfloat3(const Sdk::Vector3<T>& i_input)
  {
    return { (float)i_input.x, (float)i_input.y, (float)i_input.z };
  }

  template <typename T>
  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector3<T>& i_input)
  {
    return { (float)i_input.x, (float)i_input.y, (float)i_input.z, 1.0f };
  }

  template <typename T>
  DirectX::XMFLOAT4 getXmfloat4(const Sdk::Vector4<T>& i_input)
  {
    return { (float)i_input.x, (float)i_input.y, (float)i_input.z, (float)i_input.w };
  }


  template <typename T>
  DirectX::XMFLOAT2 getXmfloat2Norm(Sdk::Vector2<T> i_input)
  {
    i_input.normalize();
    return getXmfloat2(i_input);
  }

  template <typename T>
  DirectX::XMFLOAT3 getXmfloat3Norm(Sdk::Vector3<T> i_input)
  {
    i_input.normalize();
    return getXmfloat3(i_input);
  }

  template <typename T>
  DirectX::XMFLOAT4 getXmfloat4Norm(Sdk::Vector3<T> i_input)
  {
    i_input.normalize();
    return getXmfloat4(i_input);
  }

} // ns Dx
