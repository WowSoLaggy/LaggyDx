#pragma once

#include "VertexTypes.h"


namespace Dx
{
  /// Inverts all triangles described by the given @io_verts.
  /// Effectively changes indices and verts normals
  void invertVerts(std::vector<VertexPos3NormText>& io_verts, std::vector<int>& io_inds);


  using VertexVisitor = std::function<void(VertexPos3NormText&)>;
  void traverseVertices(std::vector<VertexPos3NormText>& io_verts, VertexVisitor i_visitor);

} // Dx
