#pragma once


namespace Dx
{
  class ILoadableResource
  {
  public:
    virtual ~ILoadableResource() = default;

    virtual void loadIfNeeded() = 0;
    virtual void unloadIfNeeded() = 0;
  };
} // ns Dx
