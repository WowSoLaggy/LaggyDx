#include "stdafx.h"
#include "Shape3dUtils.h"


namespace Dx
{
  void invertVerts(std::vector<VertexPosNormText>& io_verts, std::vector<int>& io_inds)
  {
    // Make sure that the correct number of inds is passed
    CONTRACT_EXPECT(io_inds.size() % 3 == 0);

    for (int triIndex = 0; triIndex < (int)io_inds.size(); triIndex += 3)
      std::swap(io_inds[triIndex + 1], io_inds[triIndex + 2]);

    for (auto& vert : io_verts)
      vert.normal *= -1;
  }


  void traverseVertices(std::vector<VertexPosNormText>& io_verts, VertexVisitor i_visitor)
  {
    for (auto& vert : io_verts)
      i_visitor(vert);
  }

} // Dx
