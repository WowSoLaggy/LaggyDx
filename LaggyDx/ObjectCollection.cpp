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

  void ObjectCollection::deleteObject(const IObject& i_object)
  {
    d_objects.erase(std::remove_if(d_objects.begin(), d_objects.end(),
                    [&](const auto& i_objectPtr)
                    {
                      return i_objectPtr.get() == &i_object;
                    }),
                    d_objects.end());
  }

} // ns Dx
