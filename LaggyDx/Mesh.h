#pragma once

#include "Aabb.h"
#include "IndexBuffer.h"
#include "MaterialSequence.h"
#include "TopologyTypes.h"
#include "VertexBuffer.h"


namespace Dx
{
  class Mesh
  {
  public:
    const VertexBuffer& getVertexBuffer() const;
    const IndexBuffer& getIndexBuffer() const;
    const IMaterialSequence& getMaterials() const;

    void setVertexBuffer(std::unique_ptr<VertexBuffer> i_vb);
    void setIndexBuffer(std::unique_ptr<IndexBuffer> i_ib);
    void setMaterials(std::unique_ptr<MaterialSequence> i_mats);

    void setAabb(Aabb i_aabb);
    const Aabb& getAabb() const;

    void setTopology(Topology i_topology);
    Topology getTopology() const;

  private:
    std::unique_ptr<VertexBuffer> d_vertexBuffer;
    std::unique_ptr<IndexBuffer> d_indexBuffer;
    std::unique_ptr<MaterialSequence> d_materials;
    Aabb d_aabb;

    Topology d_topology = Topology::TriangleList;
  };

} // ns Dx
