#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class IndexBuffer
  {
  public:

    int getIndexCount() const { return d_indexCount; }
    ID3D11Buffer* getPtr() const { return d_indexBuffer; }

    void create(IRenderDevice& i_renderDevice, const std::vector<int>& i_indices);
    void dispose();

  private:

    int d_indexCount;
    ID3D11Buffer* d_indexBuffer;

  };

} // ns Dx
