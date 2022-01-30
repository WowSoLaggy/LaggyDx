#pragma once

#include "IModel.h"
#include "Mesh.h"


namespace Dx
{
  class Model : public IModel
  {
  public:
    virtual const std::vector<Mesh>& getMeshes() const override;
    virtual const AnimationsMap& getAnimations() const override;
    
    void addMesh(Mesh i_mesh);
    void setAnimations(AnimationsMap i_animations);

  private:
    std::vector<Mesh> d_meshes;
    AnimationsMap d_animations;
  };

} // ns Dx
