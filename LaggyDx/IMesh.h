#pragma once

#include "Aabb.h"
#include "IndexBuffer.h"
#include "MaterialSequence.h"
#include "TopologyTypes.h"
#include "VertexBuffer.h"


namespace Dx
{
  class IMesh
  {
  public:
    virtual ~IMesh() = default;

    virtual const VertexBuffer& getVertexBuffer() const = 0;
    virtual const IndexBuffer& getIndexBuffer() const = 0;
    virtual const IMaterialSequence& getMaterials() const = 0;

    virtual void setVertexBuffer(std::unique_ptr<VertexBuffer> i_vb) = 0;
    virtual void setIndexBuffer(std::unique_ptr<IndexBuffer> i_ib) = 0;
    virtual void setMaterials(std::unique_ptr<MaterialSequence> i_mats) = 0;

    virtual void setAabb(Aabb i_aabb) = 0;
    virtual const Aabb& getAabb() const = 0;

    virtual void setTopology(Topology i_topology) = 0;
    virtual Topology getTopology() const = 0;
  };

} // ns Dx
