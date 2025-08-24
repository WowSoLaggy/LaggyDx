#pragma once

#include "IMesh.h"


namespace Dx
{
  class Mesh : public IMesh
  {
  public:
    virtual void setVertexBuffer(std::shared_ptr<VertexBuffer> i_vb) override;
    virtual const VertexBuffer& getVertexBuffer() const override;
    
    virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> i_ib) override;
    virtual const IndexBuffer& getIndexBuffer() const override;

    virtual std::vector<MaterialSpan>& getMaterials() override;
    virtual const std::vector<MaterialSpan>& getMaterials() const override;

    virtual void setAabb(Aabb i_aabb) override;
    virtual const Aabb& getAabb() const override;

    virtual void setTopology(Topology i_topology) override;
    virtual Topology getTopology() const override;

  private:
    std::shared_ptr<VertexBuffer> d_vertexBuffer;
    std::shared_ptr<IndexBuffer> d_indexBuffer;
    
    std::vector<MaterialSpan> d_materials;
    Aabb d_aabb;

    Topology d_topology = Topology::TriangleList;
  };

} // ns Dx
