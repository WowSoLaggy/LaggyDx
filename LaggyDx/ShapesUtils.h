#pragma once

#include "VertexTypes.h"


namespace Dx
{
  /// Inverts all triangles described by the given @io_verts.
  /// Effectively changes indices and verts normals
  void invertVerts(std::vector<VertexPosNormText>& io_verts, std::vector<int>& io_inds);


  /// Translates all @i_verts' position by the given @i_translation
  void translateVertsPosition(
    std::vector<VertexPosNormText>& io_verts, const Sdk::Vector3F& i_translation);

  /// Translates all @i_verts' texture coordinates by the given @i_translation
  void translateVertsTexCoords(
    std::vector<VertexPosNormText>& io_verts, const Sdk::Vector2F& i_translation);

} // Dx
