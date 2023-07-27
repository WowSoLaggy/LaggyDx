#pragma once

#include "LaggyDxFwd.h"
#include "RenderDevice.h"
#include "VertexPosNormText.h"


namespace Dx
{
  class VertexBuffer
  {
  public:
    template <typename T>
    VertexBuffer(const IRenderDevice& i_renderDevice, const std::vector<T>& i_vertices)
    {
      auto& renderDevice = dynamic_cast<const RenderDevice&>(i_renderDevice);

      d_stride = sizeof(T);
      d_verticeCount = (int)i_vertices.size();

      D3D11_BUFFER_DESC vertexBufferDesc = {};
      vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
      vertexBufferDesc.ByteWidth = sizeof(T) * (unsigned int)i_vertices.size();
      vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      vertexBufferDesc.CPUAccessFlags = 0;
      vertexBufferDesc.MiscFlags = 0;
      vertexBufferDesc.StructureByteStride = 0;

      D3D11_SUBRESOURCE_DATA vertexData = {};
      vertexData.pSysMem = i_vertices.data();
      vertexData.SysMemPitch = 0;
      vertexData.SysMemSlicePitch = 0;

      HRESULT hRes = renderDevice.getDevicePtr()->CreateBuffer(&vertexBufferDesc, &vertexData, &d_vertexBuffer);
      CONTRACT_ASSERT(!FAILED(hRes));
    }

    ~VertexBuffer();

    int getVerticeCount() const { return d_verticeCount; }
    ID3D11Buffer* getPtr() const { return d_vertexBuffer; }

    int getStride() const { return d_stride; }

  private:
    int d_stride = 0;
    int d_verticeCount = 0;
    ID3D11Buffer* d_vertexBuffer = nullptr;
  };

} // ns Dx
