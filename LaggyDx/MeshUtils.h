#pragma once

#include "LaggyDxFwd.h"
#include "Mesh.h"


namespace Dx
{
  Mesh createMeshFromShape(const IShape3d& i_shape, IRenderDevice& i_renderDevice);
  Mesh createMeshFromAabb(const Aabb& i_aabb, IRenderDevice& i_renderDevice);

} // ns Dx
