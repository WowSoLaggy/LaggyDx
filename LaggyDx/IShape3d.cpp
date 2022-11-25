#include "stdafx.h"
#include "IShape3d.h"

#include "DynamicSurface.h"
#include "Shape3d.h"


namespace Dx
{
  std::unique_ptr<IShape3d> IShape3d::fromSurface(const DynamicSurface& i_surface)
  {
    return std::make_unique<Shape3d>(i_surface.getPoints(), i_surface.getInds());
  }

} // Dx
