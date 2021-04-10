#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class ObjectCollection
  {
  public:
    std::vector<std::shared_ptr<IObject>>& getObjects() { return d_objects; }

    IObject& createObject();
    void addObject(std::shared_ptr<IObject> i_object);

  private:
    std::vector<std::shared_ptr<IObject>> d_objects;
  };

} // ns Dx
