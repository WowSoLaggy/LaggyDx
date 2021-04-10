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

  void ObjectCollection::addObject(std::shared_ptr<IObject> i_object)
  {
    d_objects.push_back(std::move(i_object));
  }

} // ns Dx
