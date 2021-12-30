#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  template <typename T>
  class ObjectCollection
  {
  public:
    std::vector<std::shared_ptr<T>>& getObjects() { return d_objects; }
    const std::vector<std::shared_ptr<T>>& getObjects() const { return d_objects; }

    void addObject(std::shared_ptr<T> i_object)
    {
      d_objects.push_back(std::move(i_object));
    }

    void deleteObject(const T& i_object)
    {
      d_objects.erase(std::remove_if(d_objects.begin(), d_objects.end(),
        [&](const auto& i_objectPtr)
        {
          return i_objectPtr.get() == &i_object;
        }),
        d_objects.end());
    }

    void deleteAllObjects()
    {
      d_objects.clear();
    }

  private:
    std::vector<std::shared_ptr<T>> d_objects;
  };

} // ns Dx
