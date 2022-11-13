#pragma once

#include "LaggyDxFwd.h"
#include "Mesh.h"


namespace Dx
{
  std::unique_ptr<IMesh> createMeshFromShape(
    const IShape3d& i_shape, IRenderDevice& i_renderDevice, bool i_addDefaultMaterial);
  std::unique_ptr<IMesh> createMeshFromAabb(const Aabb& i_aabb, IRenderDevice& i_renderDevice);

} // ns Dx
