#include "stdafx.h"
#include "Tri.h"


namespace Dx
{
  Tri::Tri(const int i_ind1, const int i_ind2, const int i_ind3)
    : ind1(i_ind1)
    , ind2(i_ind2)
    , ind3(i_ind3)
  {
  }


  int Tri::depth() const
  {
    if (parent)
      return parent->depth() + 1;
    return 1;
  }

} // ns Dx
