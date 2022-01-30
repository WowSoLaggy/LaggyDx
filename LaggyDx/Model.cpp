#include "stdafx.h"
#include "Model.h"


namespace Dx
{
  const std::vector<Mesh>& Model::getMeshes() const { return d_meshes; }
  const AnimationsMap& Model::getAnimations() const { return d_animations; }

  void Model::addMesh(Mesh i_mesh)
  {
    d_meshes.push_back(std::move(i_mesh));
  }

  void Model::setAnimations(AnimationsMap i_animations)
  {
    d_animations = std::move(i_animations);
  }

} // ns Dx
