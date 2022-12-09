#pragma once


namespace Dx
{
  template <typename T>
  class DxResourceWrapper
  {
  public:
    ~DxResourceWrapper()
    {
      if (d_resource)
      {
        d_resource->Release();
        d_resource = nullptr;
      }
    }

    T* get() const { return d_resource; }
    T** getPp() { return &d_resource; }

    T* operator->() const { return d_resource; }

    bool isNotNullptr() { return d_resource; }
    bool isNullptr() { return !isNotNullptr(); }

  private:
    T* d_resource = nullptr;
  };

};
