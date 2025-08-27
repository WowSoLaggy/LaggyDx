#pragma once

#include "IModel.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class Model : public IModel
  {
  public:
    virtual const std::vector<std::shared_ptr<IMesh>>& getMeshes() const override;
    virtual const AnimationsMap& getAnimations() const override;
    virtual const Aabb& getAabb() const override;
    virtual const IMesh& getAabbMesh() const override;

    void addMesh(std::shared_ptr<IMesh> i_mesh);
    void setAnimations(AnimationsMap i_animations);

    void createAabbMesh(IRenderDevice& i_renderDevice);

  private:
    std::vector<std::shared_ptr<IMesh>> d_meshes;
    AnimationsMap d_animations;

    Aabb d_aabb;
    std::shared_ptr<IMesh> d_aabbMesh;
    void updateAabb();
  };

} // ns Dx
