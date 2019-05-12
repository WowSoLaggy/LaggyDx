#pragma once

#include "MaterialSequence.h"
#include "VertexTypes.h"


class MeshLoader
{
public:

  static void loadInfoFromObjFile(
    const std::string& i_modelPath,
    std::vector<VertexTypePosTexNorm>& o_vertices, std::vector<int>& o_indices,
    MaterialSequence& o_matSequence);
};
