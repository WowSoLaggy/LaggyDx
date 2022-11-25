#include "stdafx.h"
#include "IndexBuffer.h"

#include "RenderDevice.h"


namespace Dx
{
  IndexBuffer::IndexBuffer(const IRenderDevice& i_renderDevice, const std::vector<int>& i_indices)
  {
    auto& renderDevice = dynamic_cast<const RenderDevice&>(i_renderDevice);

    d_indexCount = (int)i_indices.size();

    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(int) * d_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = i_indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    HRESULT hRes = renderDevice.getDevicePtr()->CreateBuffer(&indexBufferDesc, &indexData, &d_indexBuffer);
    CONTRACT_ASSERT(!FAILED(hRes));
  }

  IndexBuffer::~IndexBuffer()
  {
    d_indexBuffer->Release();
  }

} // ns Dx
