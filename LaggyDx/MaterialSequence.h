#pragma once

#include "Material.h"


struct MaterialSpan
{
  Material material;

  int startIndex;
  int count;
};

class MaterialSequence
{
public:

  void clear();
  void add(const MaterialSpan& i_materialSpan);

  const std::vector<MaterialSpan>& getMaterialSpans() const { return d_materialSpans; }

private:

  std::vector<MaterialSpan> d_materialSpans;
};
