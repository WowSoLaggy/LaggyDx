#pragma once

#include "MaterialSpan.h"
#include "VertexPosNormText.h"


namespace Dx
{
  class MeshLoader
  {
  public:

    static void loadInfoFromObjFile(
      const std::string& i_modelPath,
      std::vector<VertexPosNormText>& o_vertices, std::vector<int>& o_indices,
      std::vector<MaterialSpan>& o_matSequence);
  };

} // ns Dx
