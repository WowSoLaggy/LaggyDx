#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  std::shared_ptr<IMesh> createMeshFromShape(
    const IShape2d& i_shape, const IRenderDevice& i_renderDevice);
  std::shared_ptr<IMesh> createMeshFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::shared_ptr<IMesh> createMeshFromAabb(const Aabb& i_aabb, const IRenderDevice& i_renderDevice);

  std::shared_ptr<IModel> createModelFromMesh(std::shared_ptr<IMesh> i_mesh);

  std::shared_ptr<IObject3> createObjectFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::shared_ptr<IObject3> createObjectFromModel(std::shared_ptr<IModel> i_model);
  std::shared_ptr<IObject3> createObjectFromMesh(std::shared_ptr<IMesh> i_mesh);


  using MaterialsVisitor = std::function<void(Material& i_mat)>;
  void traverseMaterials(const IModel& i_model, MaterialsVisitor i_visitor);


} // ns Dx
