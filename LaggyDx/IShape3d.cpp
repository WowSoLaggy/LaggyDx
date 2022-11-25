#include "stdafx.h"
#include "IShape3d.h"

#include "Roam.h"
#include "Shape3d.h"


namespace Dx
{
  std::unique_ptr<IShape3d> IShape3d::fromRoam(const Roam& i_roam)
  {
    return std::make_unique<Shape3d>(i_roam.getPoints(), i_roam.getInds());
  }

} // Dx
