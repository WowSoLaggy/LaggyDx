#pragma once

#include "LaggyDxFwd.h"
#include "IModel.h"
#include "Mesh.h"


namespace Dx
{
  class Model : public IModel
  {
  public:
    virtual const std::vector<Mesh>& getMeshes() const override;
    virtual const AnimationsMap& getAnimations() const override;
    virtual const Aabb& getAabb() const override;
    virtual const Mesh& getAabbMesh() const override;

    void addMesh(Mesh i_mesh);
    void setAnimations(AnimationsMap i_animations);

    void createAabbMesh(IRenderDevice& i_renderDevice);

  private:
    std::vector<Mesh> d_meshes;
    AnimationsMap d_animations;

    Aabb d_aabb;
    Mesh d_aabbMesh;
    void updateAabb();
  };

} // ns Dx
