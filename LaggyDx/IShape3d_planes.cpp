#include "stdafx.h"
#include "IShape3d.h"

#include "Shape3d.h"


namespace Dx
{
  namespace
  {
    std::vector<VertexPosNormText> generatePlaneVerts(
      const Sdk::Size2I& i_ptsCount, const float i_dist, const float i_textureCoef)
    {
      std::vector<VertexPosNormText> verts(i_ptsCount.x * i_ptsCount.y);

      int ind = 0;
      for (int y = 0; y < i_ptsCount.y; ++y)
      {
        for (int x = 0; x < i_ptsCount.x; ++x)
        {
          VertexPosNormText p;
          p.position = { i_dist * x, 0.0f, i_dist * y };
          p.texture = { p.position.x * i_textureCoef, p.position.y * i_textureCoef };
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

  } // anonym NS


  std::unique_ptr<IShape3d> IShape3d::plane(
    const Sdk::Size2I& i_ptsCount, const float i_dist, const float i_textureCoef)
  {
    auto verts = generatePlaneVerts(i_ptsCount, i_dist, i_textureCoef);
    auto inds = generatePlaneInds(i_ptsCount);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
