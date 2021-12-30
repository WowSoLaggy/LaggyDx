#pragma once

#include "IObject.h"


namespace Dx
{
  class IObject3 : public IObject
  {
  public:
    virtual void render() const = 0;

  };

} // ns Dx
