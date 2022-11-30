#pragma once

#include "IMesh.h"
#include "IModel.h"
#include "IObject.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  std::unique_ptr<IMesh> createMeshFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::unique_ptr<IMesh> createMeshFromAabb(const Aabb& i_aabb, const IRenderDevice& i_renderDevice);

  std::unique_ptr<IModel> createModelFromMesh(std::unique_ptr<IMesh> i_mesh);

  std::unique_ptr<IObject3> createObjectFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::unique_ptr<IObject3> createObjectFromModel(std::unique_ptr<IModel> i_model);
  std::unique_ptr<IObject3> createObjectFromMesh(std::unique_ptr<IMesh> i_mesh);


  using MaterialsVisitor = std::function<void(Material& i_mat)>;
  void traverseMaterials(const IModel& i_model, MaterialsVisitor i_visitor);


} // ns Dx
