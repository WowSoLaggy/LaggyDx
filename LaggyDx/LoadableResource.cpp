#include "stdafx.h"
#include "LoadableResource.h"

#include <LaggySdk/Locker.h>


namespace Dx
{
  void LoadableResource::loadIfNeeded(IRenderDevice& i_renderDevice)
  {
    Sdk::Locker scopeLocker(*this);

    if (!d_isLoaded)
    {
      load(i_renderDevice);
      d_isLoaded = true;
    }
  }

  void LoadableResource::unloadIfNeeded()
  {
    Sdk::Locker scopeLocker(*this);

    if (d_isLoaded)
    {
      unload();
      d_isLoaded = false;
    }
  }

} // ns Dx
