#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IndexBuffer
  {
  public:
    IndexBuffer(const IRenderDevice& i_renderDevice, const std::vector<int>& i_indices);
    ~IndexBuffer();

    int getIndexCount() const { return d_indexCount; }
    ID3D11Buffer* getPtr() const { return d_indexBuffer; }

  private:
    int d_indexCount = 0;
    ID3D11Buffer* d_indexBuffer = nullptr;
  };

} // ns Dx
