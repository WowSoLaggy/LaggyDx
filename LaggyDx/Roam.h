#pragma once

#include "LaggyDxFwd.h"
#include "VertexPosNormText.h"


namespace Dx
{
  using DividerPredicate = std::function<bool(const Tri&, const std::vector<VertexPosNormText>&)>;
  using HeightPredicate = std::function<bool(const Tri&, double)>;

  class Roam
  {
  public:
    Roam(double i_size);
    Roam(double i_size, DividerPredicate i_pred);
    Roam(const HeightMap& i_heightMap, HeightPredicate i_pred);

    const std::vector<VertexPosNormText>& getPoints() const;
    const std::vector<int>& getInds() const;

    void tesselate(DividerPredicate i_pred);
    void tesselate(const HeightMap& i_heightMap, HeightPredicate i_pred);

  private:
    std::shared_ptr<Tri> d_root;

    std::vector<VertexPosNormText> d_points;
    std::vector<int> d_inds;

    void tesselate(std::shared_ptr<Tri> i_tri, DividerPredicate i_pred);
    void tesselate(std::shared_ptr<Tri> i_tri, const HeightMap& i_heightMap, HeightPredicate i_pred);
    void divideTri(std::shared_ptr<Tri> i_tri, const HeightMap* i_heightMap = nullptr);

    void setNormalsAndTexCoords();
    void calculateNormals();
    void collectInds();
    std::vector<int> collectInds(const Tri& i_tri);
  };

} // Dx
