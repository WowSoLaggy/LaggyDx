#include "stdafx.h"
#include "ObjectCollection.h"

#include "ObjectBase.h"


namespace Dx
{
  IObject& ObjectCollection::createObject()
  {
    auto& newObjectPtr = d_objects.emplace_back(std::make_shared<ObjectBase>());
    return *newObjectPtr;
  }

} // ns Dx
