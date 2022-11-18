#pragma once

#include "LoadableResource.h"


namespace Dx
{
  class IFbxResource : public LoadableResource
  {
  public:
    virtual std::shared_ptr<IModel> getModel() const = 0;
  };

} // ns Dx
