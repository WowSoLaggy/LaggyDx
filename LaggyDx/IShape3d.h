#pragma once

#include "LaggyDxFwd.h"
#include "VertexPosNormText.h"

#include <LaggySdk/Size.h>


namespace Dx
{
  class IShape3d
  {
  public:
    /// \param i_size - number of points in each dimension (not real world size!)
    /// \param i_dist - distance between two adjacent points
    /// \param i_textureCoef - additional multiplier of texture coords
    static std::unique_ptr<IShape3d> plane(
      const Sdk::Size2I& i_ptsCount, float i_dist = 1.0f, float i_textureCoef = 1.0f);
    
    /// Creates plane shape of @i_size number of points and @i_dist distance between them.
    /// Then all borders are divided twice along the border
    /// @i_borderMultiplier shall be greater than 1
    /// \param i_size - number of points in each dimension (not real world size!)
    /// \param i_dist - distance between two adjacent points
    /// \param i_textureCoef - additional multiplier of texture coords
    static std::unique_ptr<IShape3d> planeTesselatedBorder(
      const Sdk::Size2I& i_ptsCount, float i_dist = 1.0f, float i_textureCoef = 1.0f);


    static std::unique_ptr<IShape3d> cube(float i_size);

    static std::unique_ptr<IShape3d> cubeInverted(float i_size);


    /// \param i_radius - radius
    /// \param i_stackCount - number of divisions by latitude (from north to south)
    /// \param i_sliceCount - number of divisions by longitude (from west to east)
    static std::unique_ptr<IShape3d> sphere(float i_radius, int i_stackCount, int i_sliceCount);

    /// \param i_radius - radius
    /// \param i_stackCount - number of divisions by latitude (from north to south)
    /// \param i_sliceCount - number of divisions by longitude (from west to east)
    static std::unique_ptr<IShape3d> sphereInverted(float i_radius, int i_stackCount, int i_sliceCount);


    static std::unique_ptr<IShape3d> fromSurface(const DynamicSurface& i_surface);

  public:
    virtual ~IShape3d() = default;

    virtual const std::vector<VertexPosNormText>& getVerts() const = 0;
    virtual const std::vector<int>& getInds() const = 0;
    virtual const Aabb& getAabb() const = 0;
  };

} // ns Dx
