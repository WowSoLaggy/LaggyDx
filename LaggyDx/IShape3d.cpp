#include "stdafx.h"
#include "IShape3d.h"

#include "Geometry.h"
#include "Shape3d.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {
    std::vector<VertexTypePosTexNorm> generatePlaneVerts(
      const Sdk::Size2I& i_size, const float i_dist, const float i_textureCoef)
    {
      std::vector<VertexTypePosTexNorm> verts(i_size.x * i_size.y);

      int ind = 0;
      for (int y = 0; y < i_size.y; ++y)
      {
        for (int x = 0; x < i_size.x; ++x)
        {
          VertexTypePosTexNorm p;
          p.position = { (float)x * i_dist, 0.0f, (float)y * i_dist };
          p.texture = { (float)x * i_dist * i_textureCoef, (float)y * i_dist * i_textureCoef };
          p.normal = { 0.0f, 1.0f, 0.0f };

          verts[ind++] = std::move(p);
        }
      }

      return verts;
    }

    std::vector<int> generatePlaneInds(const Sdk::Size2I& i_size)
    {
      //   0 1 2 3
      // 0 . . . .
      // 1 . . . .
      // 2 . . . .
      // 3 . . . .

      const int IndsCount = (i_size.x - 1) * (i_size.y - 1) * 3 * 2;
      std::vector<int> inds(IndsCount);

      int ind = 0;
      for (int y = 0; y < i_size.y - 1; ++y)
      {
        for (int x = 0; x < i_size.x - 1; ++x)
        {
          inds[ind++] = x + y * i_size.x;
          inds[ind++] = x + (y + 1) * i_size.x;
          inds[ind++] = (x + 1) + y * i_size.x;

          inds[ind++] = (x + 1) + y * i_size.x;
          inds[ind++] = x + (y + 1) * i_size.x;
          inds[ind++] = (x + 1) + (y + 1) * i_size.x;
        }
      }

      return inds;
    }


    std::vector<VertexTypePosTexNorm> generateCubeVerts(const float i_size)
    {
      std::vector<VertexTypePosTexNorm> verts(6 * 4);

      Sdk::Vector3F normal;
      int ind = 0;

      // -X
      normal = { -1, 0, 0 };
      verts[ind].position = { 0, 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_size, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_size, 0 };
      verts[ind++].normal = normal;

      // +X
      normal = { 1, 0, 0 };
      verts[ind].position = { i_size, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, i_size };
      verts[ind++].normal = normal;


      // -Z
      normal = { 0, 0, -1 };
      verts[ind].position = { i_size, 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_size, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, 0 };
      verts[ind++].normal = normal;

      // +Z
      normal = { 0, 0, 1 };
      verts[ind].position = { 0, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_size, i_size };
      verts[ind++].normal = normal;


      // -Y
      normal = { 0, -1, 0 };
      verts[ind].position = { i_size, 0, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, 0, 0 };
      verts[ind++].normal = normal;

      // +Y
      normal = { 0, 1, 0 };
      verts[ind].position = { 0, i_size, 0 };
      verts[ind++].normal = normal;
      verts[ind].position = { 0, i_size, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, i_size };
      verts[ind++].normal = normal;
      verts[ind].position = { i_size, i_size, 0 };
      verts[ind++].normal = normal;
      
      CONTRACT_ENSURE(ind == 24);

      return verts;
    }

    std::vector<int> generateCubeInds()
    {
      std::vector<int> inds {
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

    void invertVerts(std::vector<VertexTypePosTexNorm>& io_verts, std::vector<int>& io_inds)
    {
      // Make sure that the correct number of inds is passed
      CONTRACT_EXPECT(io_inds.size() % 3 == 0);

      for (int triIndex = 0; triIndex < (int)io_inds.size(); triIndex += 3)
        std::swap(io_inds[triIndex + 1], io_inds[triIndex + 2]);

      for (auto& vert : io_verts)
        vert.normal *= -1;
    }


    std::vector<VertexTypePosTexNorm> generateSphereVerts(
      const float i_radius, const int i_stackCount, const int i_sliceCount)
    {
      std::vector<VertexTypePosTexNorm> verts;

      VertexTypePosTexNorm northPole;
      northPole.position = { 0, i_radius, 0 };
      northPole.normal = { 0, 1.0f, 0 };
      verts.push_back(std::move(northPole));

      const double phiStep = Sdk::Pi / i_stackCount;
      const double thetaStep = Sdk::Pi2 / i_sliceCount;

      for (int stack = i_stackCount - 1; stack >= 1; --stack)
      {
        // 0 theta point is included twice for a more convenient indexing
        for (int slice = 0; slice <= i_sliceCount; ++slice)
        {
          // -PiHalf because we count phi from south pole
          const auto pos = getVectorByYawAndPitch(
            thetaStep * slice, phiStep * stack - Sdk::PiHalf) * i_radius;

          VertexTypePosTexNorm p;
          p.position = pos.getVector<float>();
          p.normal = Sdk::normalize(p.position);
          verts.push_back(std::move(p));
        }
      }

      VertexTypePosTexNorm southPole;
      southPole.position = { 0, -i_radius, 0 };
      southPole.normal = { 0, -1.0f, 0 };
      verts.push_back(std::move(southPole));

      return verts;
    }

    std::vector<int> generateSphereInds(const int i_stackCount, const int i_sliceCount)
    {
      std::vector<int> inds;
      auto add = [&](const int i_val1, const int i_val2, const int i_val3) {
        inds.push_back(i_val1);
        inds.push_back(i_val2);
        inds.push_back(i_val3);
      };

      const int NorthPoleIndex = 0;
      for (int slice = 1; slice <= i_sliceCount; ++slice)
      {
        add(
          NorthPoleIndex,
          slice + 1,
          slice);
      }

      const int BaseIndex = 1; // because of north pole point
      const int pointsPerStack = i_sliceCount + 1; // because of duplicated point at each stack

      // -2 because north and south stacks are handled separately
      for (int stack = 0; stack < i_stackCount - 2; ++stack)
      {
        for (int slice = 0; slice < i_sliceCount; ++slice)
        {
          add(
            BaseIndex + stack * pointsPerStack + slice,
            BaseIndex + stack * pointsPerStack + slice + 1,
            BaseIndex + (stack + 1) * pointsPerStack + slice);
          add(
            BaseIndex + stack * pointsPerStack + slice + 1,
            BaseIndex + (stack + 1) * pointsPerStack + slice + 1,
            BaseIndex + (stack + 1) * pointsPerStack + slice);
        }
      }

      const int SouthPoleIndex = BaseIndex + (i_stackCount - 1) * pointsPerStack;
      const int SouthRingStartIndex = SouthPoleIndex - pointsPerStack;
      for (int slice = 0; slice < i_sliceCount; ++slice)
      {
        add(
          SouthPoleIndex,
          SouthRingStartIndex + slice,
          SouthRingStartIndex + slice + 1);
      }

      return inds;
    }

  } // anonym NS


  std::unique_ptr<IShape3d> IShape3d::plane(
    const Sdk::Size2I& i_size, const float i_dist, const float i_textureCoef)
  {
    auto verts = generatePlaneVerts(i_size, i_dist, i_textureCoef);
    auto inds = generatePlaneInds(i_size);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::cube(const float i_size)
  {
    auto verts = generateCubeVerts(i_size);
    auto inds = generateCubeInds();
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::cubeInverted(const float i_size)
  {
    auto verts = generateCubeVerts(i_size);
    auto inds = generateCubeInds();
    invertVerts(verts, inds);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::sphere(const float i_radius, const int i_stackCount, const int i_sliceCount)
  {
    auto verts = generateSphereVerts(i_radius, i_stackCount, i_sliceCount);
    auto inds = generateSphereInds(i_stackCount, i_sliceCount);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
