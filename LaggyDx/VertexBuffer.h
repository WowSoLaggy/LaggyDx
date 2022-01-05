#pragma once

#include "LaggyDxFwd.h"
#include "VertexTypes.h"


namespace Dx
{
  class VertexBuffer
  {
  public:
    VertexBuffer(IRenderDevice& i_renderDevice, const std::vector<VertexTypePosTexNorm>& i_vertices);
    ~VertexBuffer();

    int getVerticeCount() const { return d_verticeCount; }
    ID3D11Buffer* getPtr() const { return d_vertexBuffer; }

    int getStride() const { return sizeof(VertexTypePosTexNorm); }

  private:
    int d_verticeCount = 0;
    ID3D11Buffer* d_vertexBuffer = nullptr;
  };

} // ns Dx
