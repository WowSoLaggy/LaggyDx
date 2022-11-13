#pragma once

#include "IMesh.h"


namespace Dx
{
  class Mesh : public IMesh
  {
  public:
    virtual const VertexBuffer& getVertexBuffer() const override;
    virtual const IndexBuffer& getIndexBuffer() const override;
    virtual const IMaterialSequence& getMaterials() const override;

    virtual void setVertexBuffer(std::unique_ptr<VertexBuffer> i_vb) override;
    virtual void setIndexBuffer(std::unique_ptr<IndexBuffer> i_ib) override;
    virtual void setMaterials(std::unique_ptr<MaterialSequence> i_mats) override;

    virtual void setAabb(Aabb i_aabb) override;
    virtual const Aabb& getAabb() const override;

    virtual void setTopology(Topology i_topology) override;
    virtual Topology getTopology() const override;

  private:
    std::unique_ptr<VertexBuffer> d_vertexBuffer;
    std::unique_ptr<IndexBuffer> d_indexBuffer;
    std::unique_ptr<MaterialSequence> d_materials;
    Aabb d_aabb;

    Topology d_topology = Topology::TriangleList;
  };

} // ns Dx
