#pragma once

#include "LaggyDxFwd.h"
#include "VertexPosNormText.h"


namespace Dx
{
  using DividerPredicate = std::function<bool(const Tri&, const std::vector<VertexPosNormText>&)>;

  class Roam
  {
  public:
    Roam(double i_size, DividerPredicate i_pred);

    const std::vector<VertexPosNormText>& getPoints() const;
    const std::vector<int>& getInds() const;

  private:
    std::shared_ptr<Tri> d_root;

    std::vector<VertexPosNormText> d_points;
    std::vector<int> d_inds;

    void tesselate(DividerPredicate i_pred);
    void tesselate(std::shared_ptr<Tri> i_tri, DividerPredicate i_pred);
    void divideTri(std::shared_ptr<Tri> i_tri);

    void setNormalsAndTexCoords();
    void collectInds();
    std::vector<int> collectInds(const Tri& i_tri);
  };

} // Dx
