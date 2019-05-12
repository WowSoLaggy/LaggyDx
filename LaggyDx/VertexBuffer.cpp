#include "stdafx.h"
#include "VertexBuffer.h"

#include "RenderDevice.h"


namespace Dx
{
  void VertexBuffer::create(IRenderDevice& i_renderDevice, const std::vector<VertexTypePosTexNorm>& i_vertices)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    d_verticeCount = (int)i_vertices.size();

    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexTypePosTexNorm) * (unsigned int)i_vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = i_vertices.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    renderDevice.getDevicePtr()->CreateBuffer(&vertexBufferDesc, &vertexData, &d_vertexBuffer);
  }

  void VertexBuffer::dispose()
  {
    d_vertexBuffer->Release();
  }

} // ns Dx
