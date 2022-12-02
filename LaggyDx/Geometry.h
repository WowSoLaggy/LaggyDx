#pragma once

#include <LaggySdk/Vector.h>


namespace Dx
{
  Sdk::Vector3D getVectorByYawAndPitch(double i_yaw, double i_pitch);
  Sdk::Vector3F getYawAndPitchFromVector(const Sdk::Vector3F& i_v);

} // ns Dx
