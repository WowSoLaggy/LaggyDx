#pragma once


namespace Dx
{
  template <typename T>
  class DxResourceWrapper
  {
  public:
    ~DxResourceWrapper()
    {
      d_resource->Release();
      d_resource = nullptr;
    }

    T* get() { return d_resource; }
    T** getPp() { return &d_resource; }

    T* operator->() { return d_resource; }

    bool isNotNullptr() { return d_resource; }
    bool isNullptr() { return !isNotNullptr(); }

  private:
    T* d_resource = nullptr;
  };

};
