#pragma once

#include "IndexBuffer.h"
#include "MaterialSequence.h"
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

  private:
    std::unique_ptr<VertexBuffer> d_vertexBuffer;
    std::unique_ptr<IndexBuffer> d_indexBuffer;
    std::unique_ptr<MaterialSequence> d_materials;
  };

} // ns Dx
