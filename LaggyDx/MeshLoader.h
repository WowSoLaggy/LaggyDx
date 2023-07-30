#pragma once

#include "MaterialSpan.h"
#include "VertexTypes.h"


namespace Dx
{
  class MeshLoader
  {
  public:

    static void loadInfoFromObjFile(
      const std::string& i_modelPath,
      std::vector<VertexPos3NormText>& o_vertices, std::vector<int>& o_indices,
      std::vector<MaterialSpan>& o_matSequence);
  };

} // ns Dx
