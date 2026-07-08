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

        layout.elements.push_back(std::move(p));
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

        layout.elements.push_back(std::move(p));
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

        layout.elements.push_back(std::move(p));
      }

      return layout;
    }

    // Pos3NormText per-vertex data in slot 0 plus per-instance position/rotation/scale in slot 1.
    VertexLayout generateLayoutPos3NormTextInstanced()
    {
      VertexLayout layout = generateLayoutPos3NormText();

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "TEXCOORD";
        p.SemanticIndex = 1;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
        p.InputSlot = 1;
        p.AlignedByteOffset = 0;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
        p.InstanceDataStepRate = 1;

        layout.elements.push_back(std::move(p));
      }

      {
        D3D11_INPUT_ELEMENT_DESC p = {};
        p.SemanticName = "TEXCOORD";
        p.SemanticIndex = 2;
        p.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
        p.InputSlot = 1;
        p.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        p.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
        p.InstanceDataStepRate = 1;

        layout.elements.push_back(std::move(p));
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

        layout.elements.push_back(std::move(p));
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

        layout.elements.push_back(std::move(p));
      }

      return layout;
    }

  } // anonym NS


  const VertexLayout& getVertexLayoutPos3NormText()
  {
    static const auto layout = generateLayoutPos3NormText();
    return layout;
  }

  const VertexLayout& getVertexLayoutPos3NormTextInstanced()
  {
    static const auto layout = generateLayoutPos3NormTextInstanced();
    return layout;
  }

  const VertexLayout& getVertexLayoutPos2Text()
  {
    static const auto layout = generateLayoutPos2Text();
    return layout;
  }

} // ns Dx
