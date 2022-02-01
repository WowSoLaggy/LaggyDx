#include "stdafx.h"
#include "Mesh.h"


namespace Dx
{
  const VertexBuffer& Mesh::getVertexBuffer() const
  {
    CONTRACT_ASSERT(d_vertexBuffer);
    return *d_vertexBuffer;
  }

  const IndexBuffer& Mesh::getIndexBuffer() const
  {
    CONTRACT_ASSERT(d_indexBuffer);
    return *d_indexBuffer;
  }

  const IMaterialSequence& Mesh::getMaterials() const
  {
    CONTRACT_ASSERT(d_materials);
    return *d_materials;
  }


  void Mesh::setVertexBuffer(std::unique_ptr<VertexBuffer> i_vb)
  {
    d_vertexBuffer = std::move(i_vb);
  }

  void Mesh::setIndexBuffer(std::unique_ptr<IndexBuffer> i_ib)
  {
    d_indexBuffer = std::move(i_ib);
  }

  void Mesh::setMaterials(std::unique_ptr<MaterialSequence> i_mats)
  {
    d_materials = std::move(i_mats);
  }


  void Mesh::setAabb(Aabb i_aabb)
  {
    d_aabb = std::move(i_aabb);
  }

  const Aabb& Mesh::getAabb() const
  {
    return d_aabb;
  }


  void Mesh::setTopology(Topology i_topology) { d_topology = i_topology; }
  Topology Mesh::getTopology() const { return d_topology; }

} // ns Dx
