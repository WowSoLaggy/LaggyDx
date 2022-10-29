#pragma once

#include "Aabb.h"
#include "IShape3d.h"


namespace Dx
{
  class Shape3d : public IShape3d
  {
  public:
    Shape3d() = default;
    Shape3d(std::vector<VertexTypePosTexNorm> i_verts, std::vector<int> i_inds);

    virtual const std::vector<VertexTypePosTexNorm>& getVerts() const override;
    virtual const std::vector<int>& getInds() const override;
    virtual const Aabb& getAabb() const override;

    void setVerts(std::vector<VertexTypePosTexNorm> i_verts);
    void setInds(std::vector<int> i_inds);

  private:
    std::vector<VertexTypePosTexNorm> d_verts;
    std::vector<int> d_inds;
    Aabb d_aabb;

    void calculateAabb();
  };

} // ns Dx
