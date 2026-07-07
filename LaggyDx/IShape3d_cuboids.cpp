#include "stdafx.h"
#include "IShape3d.h"

#include "Shape3d.h"
#include "Shape3dUtils.h"


namespace Dx
{
  namespace
  {
    std::vector<VertexPos3NormText> generateCubeVerts(
      const float i_sizeX, const float i_sizeY, const float i_sizeZ)
    {
      std::vector<VertexPos3NormText> verts(6 * 4);

      Sdk::Vector3F normal;
      int ind = 0;

      // -X
      normal = { -1, 0, 0 };
      verts[ind].position = { 0, 0, 0 };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, i_sizeZ };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_sizeY, i_sizeZ };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_sizeY, 0 };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;

      // +X
      normal = { 1, 0, 0 };
      verts[ind].position = { i_sizeX, 0, i_sizeZ };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, 0, 0 };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, 0 };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, i_sizeZ };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;


      // -Z
      normal = { 0, 0, -1 };
      verts[ind].position = { i_sizeX, 0, 0 };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, 0 };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_sizeY, 0 };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, 0 };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;

      // +Z
      normal = { 0, 0, 1 };
      verts[ind].position = { 0, 0, i_sizeZ };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, 0, i_sizeZ };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, i_sizeZ };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_sizeY, i_sizeZ };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;


      // -Y
      normal = { 0, -1, 0 };
      verts[ind].position = { i_sizeX, 0, 0 };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, 0, i_sizeZ };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, i_sizeZ };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, 0 };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;

      // +Y
      normal = { 0, 1, 0 };
      verts[ind].position = { 0, i_sizeY, 0 };
      verts[ind].texture = { 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_sizeY, i_sizeZ };
      verts[ind].texture = { 0, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, i_sizeZ };
      verts[ind].texture = { 1, 1 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_sizeX, i_sizeY, 0 };
      verts[ind].texture = { 1, 0 };
      verts[ind++].normal = normal;

      CONTRACT_ENSURE(ind == 24);

      return verts;
    }

    std::vector<int> generateCubeInds()
    {
      std::vector<int> inds{
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        8, 9, 10,
        8, 10, 11,

        12, 13, 14,
        12, 14, 15,

        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23,
      };

      return inds;
    }

  } // anonym NS


  std::unique_ptr<IShape3d> IShape3d::cube(const float i_size)
  {
    return cuboid(i_size, i_size, i_size);
  }


  std::unique_ptr<IShape3d> IShape3d::cubeInverted(const float i_size)
  {
    auto verts = generateCubeVerts(i_size, i_size, i_size);
    auto inds = generateCubeInds();
    invertVerts(verts, inds);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::cuboid(
    const float i_sizeX, const float i_sizeY, const float i_sizeZ)
  {
    auto verts = generateCubeVerts(i_sizeX, i_sizeY, i_sizeZ);
    auto inds = generateCubeInds();
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
