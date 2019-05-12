#include "stdafx.h"
#include "IndexBuffer.h"

#include "RenderDevice.h"


namespace Dx
{
  void IndexBuffer::create(IRenderDevice& i_renderDevice, const std::vector<int>& i_indices)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

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

    renderDevice.getDevicePtr()->CreateBuffer(&indexBufferDesc, &indexData, &d_indexBuffer);
  }

  void IndexBuffer::dispose()
  {
    d_indexBuffer->Release();
  }

} // ns Dx
