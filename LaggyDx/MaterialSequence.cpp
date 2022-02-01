#include "stdafx.h"
#include "MaterialSequence.h"


namespace Dx
{
  void MaterialSequence::clear()
  {
    d_materialSpans.clear();
  }

  void MaterialSequence::add(MaterialSpan i_materialSpan)
  {
    d_materialSpans.push_back(std::move(i_materialSpan));
  }

} // ns Dx
