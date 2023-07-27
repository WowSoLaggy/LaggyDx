#pragma once

#include "VertexPosNormText.h"


namespace Dx
{
  /// Inverts all triangles described by the given @io_verts.
  /// Effectively changes indices and verts normals
  void invertVerts(std::vector<VertexPosNormText>& io_verts, std::vector<int>& io_inds);


  using VertexVisitor = std::function<void(VertexPosNormText&)>;
  void traverseVertices(std::vector<VertexPosNormText>& io_verts, VertexVisitor i_visitor);

} // Dx
