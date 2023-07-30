#pragma once

#include "Aabb.h"
#include "IShape3d.h"


namespace Dx
{
  class Shape3d : public IShape3d
  {
  public:
    Shape3d() = default;
    Shape3d(std::vector<VertexPos3NormText> i_verts, std::vector<int> i_inds);

    virtual const std::vector<VertexPos3NormText>& getVerts() const override;
    virtual const std::vector<int>& getInds() const override;
    virtual const Aabb& getAabb() const override;

    void setVerts(std::vector<VertexPos3NormText> i_verts);
    void setInds(std::vector<int> i_inds);

  private:
    std::vector<VertexPos3NormText> d_verts;
    std::vector<int> d_inds;
    Aabb d_aabb;

    void calculateAabb();
  };

} // ns Dx
