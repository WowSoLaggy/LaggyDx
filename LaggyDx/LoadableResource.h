#pragma once

#include "ILoadableResource.h"

#include <LaggySdk/ILockable.h>


namespace Dx
{
  class LoadableResource : public ILoadableResource, public Sdk::ILockable
  {
  public:
    virtual void loadIfNeeded(IRenderDevice& i_renderDevice) override final;
    virtual void unloadIfNeeded() override final;

  protected:
    virtual void load(IRenderDevice& i_renderDevice) = 0;
    virtual void unload() = 0;

  private:
    bool d_isLoaded = false;
  };

} // ns Dx
