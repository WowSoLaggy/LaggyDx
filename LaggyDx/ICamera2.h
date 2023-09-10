#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ICamera2
  {
  public:
    static std::unique_ptr<ICamera2> create();

  public:
    virtual ~ICamera2() = default;

    virtual const Sdk::Vector2I& getOffset() const = 0;
    virtual void setOffset(Sdk::Vector2I i_offset) = 0;
  };

} // ns Dx
