#pragma once

#include "IMesh.h"
#include "IModel.h"
#include "IObject.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  std::unique_ptr<IMesh> createMeshFromShape(
    const IShape3d& i_shape, IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::unique_ptr<IMesh> createMeshFromAabb(const Aabb& i_aabb, IRenderDevice& i_renderDevice);

  std::unique_ptr<IModel> createModelFromMesh(std::unique_ptr<IMesh> i_mesh);

  std::unique_ptr<IObject3> createObjectFromShape(
    const IShape3d& i_shape, IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::unique_ptr<IObject3> createObjectFromModel(std::unique_ptr<IModel> i_model);
  std::unique_ptr<IObject3> createObjectFromMesh(std::unique_ptr<IMesh> i_mesh);


  void setColorOfAllMaterials(const IModel& i_model, const Sdk::Vector4F& i_color);


} // ns Dx
