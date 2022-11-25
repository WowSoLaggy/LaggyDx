#pragma once

#include "LaggyDxFwd.h"
#include "VertexPosNormText.h"


namespace Dx
{
  class Roam
  {
  public:
    Roam(double i_size);

    const std::vector<VertexPosNormText>& getPoints() const;
    const std::vector<int>& getInds() const;

  private:
    std::shared_ptr<Tri> tri1;
    std::shared_ptr<Tri> tri2;

    std::vector<VertexPosNormText> d_points;
    std::vector<int> d_inds;

    void divideTri(std::shared_ptr<Tri> i_tri);

    void setNormalsAndTexCoords();
    void collectInds();
    std::vector<int> collectInds(const Tri& i_tri);
  };

} // Dx
