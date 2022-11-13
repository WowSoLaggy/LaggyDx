#include "stdafx.h"
#include "Mesh.h"


namespace Dx
{
  void Mesh::setVertexBuffer(std::unique_ptr<VertexBuffer> i_vb)
  {
    d_vertexBuffer = std::move(i_vb);
  }
  const VertexBuffer& Mesh::getVertexBuffer() const
  {
    CONTRACT_ASSERT(d_vertexBuffer);
    return *d_vertexBuffer;
  }

  void Mesh::setIndexBuffer(std::unique_ptr<IndexBuffer> i_ib)
  {
    d_indexBuffer = std::move(i_ib);
  }
  const IndexBuffer& Mesh::getIndexBuffer() const
  {
    CONTRACT_ASSERT(d_indexBuffer);
    return *d_indexBuffer;
  }

  std::vector<MaterialSpan>& Mesh::getMaterials()
  {
    return d_materials;
  }

  const std::vector<MaterialSpan>& Mesh::getMaterials() const
  {
    return d_materials;
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
