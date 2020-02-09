#include "stdafx.h"
#include "VertexShaderResource.h"

#include "RenderDevice.h"

#include <Sdk/StringUtils.h>


namespace Dx
{
  VertexShaderResource::VertexShaderResource(std::string i_shaderFilePath)
    : d_shaderFilePath(std::move(i_shaderFilePath))
  {
  }


  void VertexShaderResource::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    // Vertex Shader

    ID3D10Blob* vertexShaderBuffer = nullptr;
    D3DReadFileToBlob(Sdk::getWString(d_shaderFilePath).c_str(), &vertexShaderBuffer);

    renderDevice.getDevicePtr()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
      vertexShaderBuffer->GetBufferSize(), NULL, &d_vertexShader);

    // Input layout

    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    renderDevice.getDevicePtr()->CreateInputLayout(polygonLayout, numElements,
      vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &d_layout);

    vertexShaderBuffer->Release();

    d_loaded = true;
  }

  void VertexShaderResource::unload()
  {
    d_loaded = false;

    d_layout->Release();
    d_vertexShader->Release();
  }

} // ns Dx
