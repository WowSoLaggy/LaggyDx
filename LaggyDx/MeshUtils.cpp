#include "stdafx.h"
#include "MeshUtils.h"

#include "Aabb.h"
#include "IShape3d.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


namespace Dx
{
  Mesh createMeshFromShape(const IShape3d& i_shape, IRenderDevice& i_renderDevice)
  {
    Mesh mesh;
    mesh.setVertexBuffer(std::make_unique<VertexBuffer>(i_renderDevice, i_shape.getVerts()));
    mesh.setIndexBuffer(std::make_unique<IndexBuffer>(i_renderDevice, i_shape.getInds()));
    mesh.setAabb(i_shape.getAabb());
    return mesh;
  }

  Mesh createMeshFromAabb(const Aabb& i_aabb, IRenderDevice& i_renderDevice)
  {
    Mesh mesh;

    std::vector<VertexPosNormText> verts;
    std::vector<int> inds;

    auto add = [&](Sdk::Vector3F i_pos)
    {
      VertexPosNormText vert;
      vert.position = std::move(i_pos);
      verts.push_back(std::move(vert));
    };

    // min Y face

    add({ i_aabb.getMinX(), i_aabb.getMinY(), i_aabb.getMinZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMinY(), i_aabb.getMinZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMinY(), i_aabb.getMaxZ() });
    add({ i_aabb.getMinX(), i_aabb.getMinY(), i_aabb.getMaxZ() });

    // max Y face

    add({ i_aabb.getMaxX(), i_aabb.getMaxY(), i_aabb.getMinZ() });
    add({ i_aabb.getMinX(), i_aabb.getMaxY(), i_aabb.getMinZ() });
    add({ i_aabb.getMinX(), i_aabb.getMaxY(), i_aabb.getMaxZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMaxY(), i_aabb.getMaxZ() });

    // inds

    inds.insert(inds.begin(), {
      0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 5, 3, 6, 1, 4, 2, 7
      });

    // misc

    mesh.setVertexBuffer(std::make_unique<VertexBuffer>(i_renderDevice, verts));
    mesh.setIndexBuffer(std::make_unique<IndexBuffer>(i_renderDevice, inds));
    mesh.setAabb(i_aabb);

    MaterialSpan matSpan;
    matSpan.startIndex = 0;
    matSpan.count = (int)inds.size();
    matSpan.material.textureName = "white.png";

    MaterialSequence matSeq;
    matSeq.add(std::move(matSpan));
    mesh.setMaterials(std::make_unique<MaterialSequence>(std::move(matSeq)));

    mesh.setTopology(Topology::LineList);

    return mesh;
  }

} // ns Dx
