#include "stdafx.h"
#include "MaterialSequence.h"


namespace Dx
{
  void MaterialSequence::clear()
  {
    d_materialSpans.clear();
  }

  void MaterialSequence::add(const MaterialSpan& i_materialSpan)
  {
    d_materialSpans.push_back(i_materialSpan);
  }

} // ns Dx
