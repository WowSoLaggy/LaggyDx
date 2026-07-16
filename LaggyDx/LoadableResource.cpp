#include "stdafx.h"
#include "LoadableResource.h"

#include <LaggySdk/Locker.h>


namespace Dx
{
  LoadableResource::~LoadableResource()
  {
    unloadIfNeeded();
  }

  void LoadableResource::loadIfNeeded()
  {
    Sdk::Locker scopeLocker(*this);

    if (!d_isLoaded)
    {
      load();
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
