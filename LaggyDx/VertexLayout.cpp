#include "stdafx.h"
#include "VertexLayout.h"


namespace Dx
{
  namespace
  {
    VertexLayout generateLayoutPos3NormText()
    {
      VertexLayout layout;

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "POSITION";
        p.SemanticIndex = 0;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
        p.InputSlot = 0;
        p.AlignedByteOffset = 0;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        p.InstanceDataStepRate = 0;

        layout.push_back(std::move(p));
      }

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "NORMAL";
        p.SemanticIndex = 0;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
        p.InputSlot = 0;
        p.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        p.InstanceDataStepRate = 0;

        layout.push_back(std::move(p));
      }

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "TEXCOORD";
        p.SemanticIndex = 0;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        p.InputSlot = 0;
        p.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        p.InstanceDataStepRate = 0;

        layout.push_back(std::move(p));
      }

      return layout;
    }

    VertexLayout generateLayoutPos2Text()
    {
      VertexLayout layout;

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "SV_POSITION";
        p.SemanticIndex = 0;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        p.InputSlot = 0;
        p.AlignedByteOffset = 0;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        p.InstanceDataStepRate = 0;

        layout.push_back(std::move(p));
      }

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "TEXCOORD";
        p.SemanticIndex = 0;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        p.InputSlot = 0;
        p.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
        p.InstanceDataStepRate = 0;

        layout.push_back(std::move(p));
      }

      return layout;
    }

  } // anonym NS


  const VertexLayout& getVertexLayoutPos3NormText()
  {
    static const auto layout = generateLayoutPos3NormText();
    return layout;
  }

  const VertexLayout& getVertexLayoutPos2Text()
  {
    static const auto layout = generateLayoutPos2Text();
    return layout;
  }

} // ns Dx
