#include "stdafx.h"
#include "IShape3d.h"

#include "Geometry.h"
#include "Shape3d.h"
#include "ShapesUtils.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {
    std::vector<VertexPosNormText> generateSphereVerts(
      const float i_radius, const int i_stackCount, const int i_sliceCount)
    {
      std::vector<VertexPosNormText> verts;

      VertexPosNormText northPole;
      northPole.position = { 0, i_radius, 0 };
      northPole.normal = { 0, 1.0f, 0 };
      northPole.texture = { 0, 0 };
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

          VertexPosNormText p;
          p.position = pos.getVector<float>();
          p.normal = Sdk::normalize(p.position);
          p.texture = { (float)slice / i_sliceCount, (float)(i_stackCount - stack) / i_stackCount };
          verts.push_back(std::move(p));
        }
      }

      VertexPosNormText southPole;
      southPole.position = { 0, -i_radius, 0 };
      southPole.normal = { 0, -1.0f, 0 };
      southPole.texture = { 0, 1 };
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


  std::unique_ptr<IShape3d> IShape3d::sphere(const float i_radius, const int i_stackCount, const int i_sliceCount)
  {
    auto verts = generateSphereVerts(i_radius, i_stackCount, i_sliceCount);
    auto inds = generateSphereInds(i_stackCount, i_sliceCount);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::sphereInverted(float i_radius, int i_stackCount, int i_sliceCount)
  {
    auto verts = generateSphereVerts(i_radius, i_stackCount, i_sliceCount);
    auto inds = generateSphereInds(i_stackCount, i_sliceCount);
    invertVerts(verts, inds);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
