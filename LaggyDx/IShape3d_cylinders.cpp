#include "stdafx.h"
#include "IShape3d.h"

#include "Shape3d.h"

#include <LaggySdk/Math.h>


namespace Dx
{
  namespace
  {
    // A cylinder aligned to the Y axis: base ring at y = 0, top ring at y = i_height,
    // so the shape is base-anchored on the ground (matches cube(1) spanning [0, size]).
    // Side wall + a top and bottom cap fan. i_sliceCount segments around the axis.
    std::vector<VertexPos3NormText> generateCylinderVerts(
      const float i_radius, const float i_height, const int i_sliceCount, const float i_textureCoeff)
    {
      std::vector<VertexPos3NormText> verts;

      const double thetaStep = Sdk::Pi2 / i_sliceCount;

      // Side wall: a bottom and top ring. The 0-theta point is duplicated at the end
      // (slice == i_sliceCount) so the wrap-around quad has its own UVs and the side
      // normals stay radial without sharing a seam vertex.
      for (int slice = 0; slice <= i_sliceCount; ++slice)
      {
        const double theta = thetaStep * slice;
        const float c = (float)std::cos(theta);
        const float s = (float)std::sin(theta);
        const Sdk::Vector3F radial{ c, 0.0f, s };
        const float u = (float)slice / i_sliceCount * i_textureCoeff;

        VertexPos3NormText bottom;
        bottom.position = { c * i_radius, 0.0f, s * i_radius };
        bottom.normal = radial;
        bottom.texture = { u, 1.0f * i_textureCoeff };
        verts.push_back(std::move(bottom));

        VertexPos3NormText top;
        top.position = { c * i_radius, i_height, s * i_radius };
        top.normal = radial;
        top.texture = { u, 0.0f };
        verts.push_back(std::move(top));
      }

      // Cap centres + cap rings (own vertices so cap normals are flat ±Y).
      VertexPos3NormText topCentre;
      topCentre.position = { 0.0f, i_height, 0.0f };
      topCentre.normal = { 0, 1.0f, 0 };
      topCentre.texture = { 0.5f, 0.5f };
      verts.push_back(std::move(topCentre));

      VertexPos3NormText bottomCentre;
      bottomCentre.position = { 0.0f, 0.0f, 0.0f };
      bottomCentre.normal = { 0, -1.0f, 0 };
      bottomCentre.texture = { 0.5f, 0.5f };
      verts.push_back(std::move(bottomCentre));

      for (int slice = 0; slice <= i_sliceCount; ++slice)
      {
        const double theta = thetaStep * slice;
        const float c = (float)std::cos(theta);
        const float s = (float)std::sin(theta);

        VertexPos3NormText topRing;
        topRing.position = { c * i_radius, i_height, s * i_radius };
        topRing.normal = { 0, 1.0f, 0 };
        topRing.texture = { (c * 0.5f + 0.5f), (s * 0.5f + 0.5f) };
        verts.push_back(std::move(topRing));

        VertexPos3NormText bottomRing;
        bottomRing.position = { c * i_radius, 0.0f, s * i_radius };
        bottomRing.normal = { 0, -1.0f, 0 };
        bottomRing.texture = { (c * 0.5f + 0.5f), (s * 0.5f + 0.5f) };
        verts.push_back(std::move(bottomRing));
      }

      return verts;
    }

    std::vector<int> generateCylinderInds(const int i_sliceCount)
    {
      std::vector<int> inds;
      auto add = [&](const int i_v1, const int i_v2, const int i_v3) {
        inds.push_back(i_v1);
        inds.push_back(i_v2);
        inds.push_back(i_v3);
      };

      // Side wall: two interleaved rings, 2 verts per slice (bottom, top).
      for (int slice = 0; slice < i_sliceCount; ++slice)
      {
        const int b0 = slice * 2;
        const int t0 = b0 + 1;
        const int b1 = b0 + 2;
        const int t1 = b0 + 3;
        add(b0, t0, b1);
        add(t0, t1, b1);
      }

      // Cap vertices laid out after the (i_sliceCount + 1) side pairs.
      const int sideVerts = (i_sliceCount + 1) * 2;
      const int topCentre = sideVerts;
      const int bottomCentre = sideVerts + 1;
      const int capRingStart = sideVerts + 2; // pairs of (topRing, bottomRing) per slice

      for (int slice = 0; slice < i_sliceCount; ++slice)
      {
        const int topRing0 = capRingStart + slice * 2;
        const int bottomRing0 = topRing0 + 1;
        const int topRing1 = capRingStart + (slice + 1) * 2;
        const int bottomRing1 = topRing1 + 1;

        // Cap winding matches the side wall's front-face convention so the caps
        // render outward (top faces +Y, bottom faces -Y) rather than showing their
        // back face (which culls and lets you see inside the pole).
        add(topCentre, topRing1, topRing0);
        add(bottomCentre, bottomRing0, bottomRing1);
      }

      return inds;
    }

  } // anonym NS


  std::unique_ptr<IShape3d> IShape3d::cylinder(
    const float i_radius, const float i_height, const int i_sliceCount, const float i_textureCoeff /* = 1.0f */)
  {
    auto verts = generateCylinderVerts(i_radius, i_height, i_sliceCount, i_textureCoeff);
    auto inds = generateCylinderInds(i_sliceCount);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
