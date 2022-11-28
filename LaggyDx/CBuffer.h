#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class CBuffer
  {
  public:
    CBuffer(const RenderDevice& i_renderDevice, int i_byteWidth);
    ~CBuffer();

    ID3D11Buffer* get() const;
    ID3D11Buffer* const* getPp() const;

  private:
    ID3D11Buffer* d_buffer = nullptr;
  };

} // ns Dx
