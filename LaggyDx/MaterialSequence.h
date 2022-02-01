#pragma once

#include "IMaterialSequence.h"


namespace Dx
{
  class MaterialSequence : public IMaterialSequence
  {
  public:
    void clear();
    void add(MaterialSpan i_materialSpan);

    virtual const std::vector<MaterialSpan>& getMaterialSpans() const override { return d_materialSpans; }

  private:
    std::vector<MaterialSpan> d_materialSpans;
  };

} // ns Dx
