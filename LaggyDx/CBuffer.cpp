#include "stdafx.h"
#include "CBuffer.h"

#include "RenderDevice.h"


namespace Dx
{
  CBuffer::CBuffer(const RenderDevice& i_renderDevice, const int i_byteWidth)
  {
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = i_byteWidth;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    HRESULT hRes = i_renderDevice.getDevicePtr()->CreateBuffer(&desc, nullptr, &d_buffer);
    CONTRACT_ASSERT(!FAILED(hRes));
  }

  CBuffer::~CBuffer()
  {
    d_buffer->Release();
    d_buffer = nullptr;
  }
  

  ID3D11Buffer* CBuffer::get() const
  {
    return d_buffer;
  }

  ID3D11Buffer* const* CBuffer::getPp() const
  {
    return &d_buffer;
  }

} // ns Dx
