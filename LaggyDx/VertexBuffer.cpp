#include "stdafx.h"
#include "VertexBuffer.h"


namespace Dx
{
  VertexBuffer::~VertexBuffer()
  {
    d_vertexBuffer->Release();
  }

} // ns Dx
