#pragma once


namespace Dx
{
  class VertexLayout : public std::vector<D3D11_INPUT_ELEMENT_DESC>
  {
    // TODO: ae
    // Change inheritance to aggregation
  };

  const VertexLayout& getVertexLayoutPos3NormText();
  const VertexLayout& getVertexLayoutPos2Text();

} // ns Dx
