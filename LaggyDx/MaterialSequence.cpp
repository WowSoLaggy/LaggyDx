#include "stdafx.h"
#include "MaterialSequence.h"


void MaterialSequence::clear()
{
  d_materialSpans.clear();
}

void MaterialSequence::add(const MaterialSpan& i_materialSpan)
{
  d_materialSpans.push_back(i_materialSpan);
}
