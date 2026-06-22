#include "stdafx.h"
#include "HeightMapUtils.h"

#include "HeightMap.h"
#include "ICamera3.h"


namespace Dx
{
  namespace
  {
    // Heightfield ray-march for picking. The march is clipped to the terrain's 3D
    // AABB first (see below), so the step only ever walks the slab the terrain can
    // actually occupy. One unit is one heightmap cell, so a sub-cell step brackets
    // the surface crossing tightly; bisection then refines the bracket against the
    // real (bilinear) surface to sub-cell accuracy.
    constexpr float PickStep = 0.5f;
    constexpr int PickRefineIterations = 8;

    // Clips a ray against an axis-aligned box [i_min, i_max] using the slab method,
    // returning the entry/exit ray parameters (clamped so o_tEnter >= 0 - we never
    // pick behind the camera). Returns false if the ray misses the box.
    bool clipRayToAabb(
      const Sdk::Vector3F& i_origin,
      const Sdk::Vector3F& i_dirInv,
      const Sdk::Vector3F& i_min,
      const Sdk::Vector3F& i_max,
      float& o_tEnter,
      float& o_tExit)
    {
      // Per-axis slab intersections; i_dirInv may be +/-inf for an axis-parallel
      // ray, which the min/max below handle correctly (the inf-scaled slabs drop
      // out as long as the origin is inside that axis' extent).
      const float tx1 = (i_min.x - i_origin.x) * i_dirInv.x;
      const float tx2 = (i_max.x - i_origin.x) * i_dirInv.x;
      const float ty1 = (i_min.y - i_origin.y) * i_dirInv.y;
      const float ty2 = (i_max.y - i_origin.y) * i_dirInv.y;
      const float tz1 = (i_min.z - i_origin.z) * i_dirInv.z;
      const float tz2 = (i_max.z - i_origin.z) * i_dirInv.z;

      float tEnter = std::min(tx1, tx2);
      float tExit = std::max(tx1, tx2);
      tEnter = std::max(tEnter, std::min(ty1, ty2));
      tExit = std::min(tExit, std::max(ty1, ty2));
      tEnter = std::max(tEnter, std::min(tz1, tz2));
      tExit = std::min(tExit, std::max(tz1, tz2));

      if (tExit < tEnter || tExit < 0.0f)
        return false;

      o_tEnter = std::max(tEnter, 0.0f);
      o_tExit = tExit;
      return true;
    }
  }


  std::optional<Sdk::Vector3F> pickPoint(
    const HeightMap& i_heightMap,
    const ICamera3& i_camera,
    const Sdk::Vector2I& i_screenPoint)
  {
    const auto ray = i_camera.screenToWorld(i_screenPoint);

    const auto& origin = ray.getPoint();
    const auto& dir = ray.getDir();
    const auto& dirInv = ray.getDirInv();

    const float maxX = (float)(i_heightMap.getWidth() - 1);
    const float maxZ = (float)(i_heightMap.getHeight() - 1);
    const float minY = (float)i_heightMap.getMinHeight();
    const float maxY = (float)i_heightMap.getMaxHeight();

    // Clip to the terrain's full 3D AABB, not just its XZ footprint: a ray that
    // looks nearly parallel to the ground toward a mountain spends a long span
    // above maxHeight before it can possibly hit anything. Clipping in Y starts
    // the march near the terrain volume instead of at the far map wall, and makes
    // the "first sample already below surface" shortcut below correct (at tEnter
    // we're on the box, not still up in the sky).
    float tEnter = 0.0f;
    float tExit = 0.0f;
    if (!clipRayToAabb(origin, dirInv, { 0.0f, minY, 0.0f }, { maxX, maxY, maxZ }, tEnter, tExit))
      return std::nullopt;

    const auto pointAt = [&](const float t) {
      return Sdk::Vector3F{ origin.x + dir.x * t, origin.y + dir.y * t, origin.z + dir.z * t };
    };

    // Height of the ray above the terrain surface at parameter t. Both X and Z are
    // guaranteed in-bounds for t in [tEnter, tExit] (we clipped to the XZ extent),
    // but clamp defensively against floating-point drift at the interval ends.
    const auto surfaceDiff = [&](const float t) {
      const float x = std::clamp(origin.x + dir.x * t, 0.0f, maxX);
      const float z = std::clamp(origin.z + dir.z * t, 0.0f, maxZ);
      return (origin.y + dir.y * t) - (float)i_heightMap.getHeight((double)x, (double)z);
    };

    // If the ray enters the terrain volume already at or below the surface (grazing
    // or steep rays whose first in-bounds sample is underground), the entry point is
    // the hit - there is no above->below transition to wait for.
    float prevT = tEnter;
    float prevDiff = surfaceDiff(tEnter);
    if (prevDiff <= 0.0f)
      return pointAt(tEnter);

    for (float t = tEnter + PickStep; ; t += PickStep)
    {
      // Clamp the final sample exactly to tExit so we never miss a crossing that
      // happens in the last partial step, then stop after processing it.
      const bool last = t >= tExit;
      if (last)
        t = tExit;

      const float diff = surfaceDiff(t);
      if (diff <= 0.0f && prevDiff > 0.0f)
      {
        // Crossing bracketed in [prevT, t]. Bisect against the real bilinear surface
        // (the single linear lerp the old code used assumed a linear surface across
        // the step, which it isn't), keeping the half that still straddles zero.
        float lo = prevT;
        float hi = t;
        for (int i = 0; i < PickRefineIterations; ++i)
        {
          const float mid = 0.5f * (lo + hi);
          if (surfaceDiff(mid) > 0.0f)
            lo = mid;
          else
            hi = mid;
        }
        return pointAt(0.5f * (lo + hi));
      }

      if (last)
        break;

      prevT = t;
      prevDiff = diff;
    }

    return std::nullopt;
  }

} // ns Dx
