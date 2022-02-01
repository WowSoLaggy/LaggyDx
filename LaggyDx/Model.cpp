#include "stdafx.h"
#include "Model.h"

#include "MeshUtils.h"


namespace Dx
{
  const std::vector<Mesh>& Model::getMeshes() const { return d_meshes; }
  const AnimationsMap& Model::getAnimations() const { return d_animations; }
  const Aabb& Model::getAabb() const { return d_aabb; }
  const Mesh& Model::getAabbMesh() const { return d_aabbMesh; }


  void Model::addMesh(Mesh i_mesh)
  {
    d_meshes.push_back(std::move(i_mesh));
    updateAabb();
  }

  void Model::setAnimations(AnimationsMap i_animations)
  {
    d_animations = std::move(i_animations);
  }


  void Model::updateAabb()
  {
    d_aabb = Aabb();

    if (d_meshes.empty())
      return;

    d_aabb = d_meshes.front().getAabb();

    for (const auto& mesh : d_meshes)
      d_aabb.mergeWith(mesh.getAabb());
  }

  void Model::createAabbMesh(IRenderDevice& i_renderDevice)
  {
    d_aabbMesh = createMeshFromAabb(d_aabb, i_renderDevice);
  }

} // ns Dx
