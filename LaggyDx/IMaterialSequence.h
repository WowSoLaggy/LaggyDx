#pragma once

#include "MaterialSpan.h"


namespace Dx
{
  class IMaterialSequence
  {
  public:
    virtual ~IMaterialSequence() = default;

    virtual const std::vector<MaterialSpan>& getMaterialSpans() const = 0;
  };

} // ns Dx
