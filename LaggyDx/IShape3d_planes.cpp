#include "stdafx.h"
#include "IShape3d.h"

#include "Shape3d.h"
#include "ShapesUtils.h"


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


    std::vector<VertexPosNormText> generateRectVerts(
      const Sdk::Size2I& i_ptsCount, const float i_dist, const float i_textureCoef)
    {
      std::vector<VertexPosNormText> verts;

      for (int x = 0; x < i_ptsCount.x; ++x)
      {
        VertexPosNormText p0;
        p0.position = { i_dist * x, 0, 0 };
        p0.texture = { p0.position.x * i_textureCoef, 0 };
        p0.normal = { 0.0f, 1.0f, 0.0f };
        verts.push_back(std::move(p0));
      }

      for (int x = 0; x < i_ptsCount.x; ++x)
      {
        VertexPosNormText p1;
        p1.position = { i_dist * x, 0, i_dist * (i_ptsCount.y - 1)};
        p1.texture = { p1.position.x * i_textureCoef, p1.position.y * i_textureCoef };
        p1.normal = { 0.0f, 1.0f, 0.0f };
        verts.push_back(std::move(p1));
      }

      for (int y = 0; y < i_ptsCount.y; ++y)
      {
        VertexPosNormText p0;
        p0.position = { 0, 0, i_dist * y };
        p0.texture = { 0, p0.position.y * i_textureCoef };
        p0.normal = { 0.0f, 1.0f, 0.0f };
        verts.push_back(std::move(p0));
      }

      for (int y = 0; y < i_ptsCount.y; ++y)
      {
        VertexPosNormText p1;
        p1.position = { i_dist * (i_ptsCount.x - 1), 0, i_dist * y };
        p1.texture = { p1.position.x * i_textureCoef, p1.position.y * i_textureCoef };
        p1.normal = { 0.0f, 1.0f, 0.0f };
        verts.push_back(std::move(p1));
      }

      return verts;
    }


    void addBorderInds(std::vector<int>& io_inds, const Sdk::Size2I& i_ptsCount, const int i_base)
    {
      auto add = [&](const int i_ind1, const int i_ind2, const int i_ind3) {
        io_inds.push_back(i_ind1);
        io_inds.push_back(i_ind2);
        io_inds.push_back(i_ind3);
      };

      const auto ptsCountNoBorders = i_ptsCount - Sdk::Size2I{ 2, 2 };
      
      const int baseX0 = i_base + 2;
      const int baseX1 = i_base + (i_ptsCount.x * 2 - 1) + 2;

      const int x0 = 0;
      const int x1 = ptsCountNoBorders.x * (ptsCountNoBorders.y - 1);

      for (int i = 0; i < ptsCountNoBorders.x - 1; ++i)
      {
        add(x0 + i, baseX0 + i * 2 + 1, baseX0 + i * 2);
        add(x0 + i + 1, baseX0 + i * 2 + 2, baseX0 + i * 2 + 1);
        add(x0 + i, x0 + i + 1, baseX0 + i * 2 + 1);

        add(x1 + i, baseX1 + i * 2, baseX1 + i * 2 + 1);
        add(x1 + i + 1, baseX1 + i * 2 + 1, baseX1 + i * 2 + 2);
        add(x1 + i, baseX1 + i * 2 + 1, x1 + i + 1);
      }

      const int baseY0 = i_base + (i_ptsCount.x * 2 - 1) * 2 + 2;
      const int baseY1 = i_base + (i_ptsCount.x * 2 - 1) * 2 + (i_ptsCount.y * 2 - 1) + 2;

      const int y0 = 0;
      const int y1 = ptsCountNoBorders.x - 1;

      for (int i = 0; i < ptsCountNoBorders.y - 1; ++i)
      {
        add(y0 + i * ptsCountNoBorders.x, baseY0 + i * 2, baseY0 + i * 2 + 1);
        add(y0 + (i + 1) * ptsCountNoBorders.x, baseY0 + i * 2 + 1, baseY0 + i * 2 + 2);
        add(y0 + i * ptsCountNoBorders.x, baseY0 + i * 2 + 1, y0 + (i + 1) * ptsCountNoBorders.x);

        add(y1 + i * ptsCountNoBorders.x, baseY1 + i * 2 + 1, baseY1 + i * 2);
        add(y1 + (i + 1) * ptsCountNoBorders.x, baseY1 + i * 2 + 2, baseY1 + i * 2 + 1);
        add(y1 + i * ptsCountNoBorders.x, y1 + (i + 1) * ptsCountNoBorders.x, baseY1 + i * 2 + 1);
      }

      // Finally, corners

      // x0-y0
      add(baseX0 - 1, baseX0 - 2, baseY0 - 1);
      add(0, baseX0, baseX0 - 1);
      add(0, baseX0 - 1, baseY0 - 1);
      add(0, baseY0 - 1, baseY0);

      // x1-y0
      add(baseY1 - 2, baseX0 + (ptsCountNoBorders.x * 2) - 1, baseY1 - 1);
      add(ptsCountNoBorders.x - 1, baseY1, baseY1 - 1);
      add(ptsCountNoBorders.x - 1, baseY1 - 1, baseX0 + (ptsCountNoBorders.x * 2) - 1);
      add(ptsCountNoBorders.x - 1, baseX0 + (ptsCountNoBorders.x * 2) - 1, baseX0 + (ptsCountNoBorders.x * 2) - 2);

      // x0-y1
      add(baseX1 - 2, baseX1 - 1, baseY0 + (ptsCountNoBorders.y * 2) - 1);
      add(
        ptsCountNoBorders.x * (ptsCountNoBorders.y - 1),
        baseY0 + (ptsCountNoBorders.y * 2) - 2,
        baseY0 + (ptsCountNoBorders.y * 2) - 1);
      add(
        ptsCountNoBorders.x * (ptsCountNoBorders.y - 1),
        baseY0 + (ptsCountNoBorders.y * 2) - 1,
        baseX1 - 1);
      add(
        ptsCountNoBorders.x * (ptsCountNoBorders.y - 1),
        baseX1 - 1,
        baseX1);

      // x1-y1
      add(
        baseX1 + (ptsCountNoBorders.y * 2) - 1,
        baseX1 + (ptsCountNoBorders.y * 2),
        baseY1 + (ptsCountNoBorders.x * 2) - 1);
      add(
        ptsCountNoBorders.x * ptsCountNoBorders.y - 1,
        baseY1 + (ptsCountNoBorders.x * 2) - 1,
        baseY1 + (ptsCountNoBorders.x * 2) - 2);
      add(
        ptsCountNoBorders.x * ptsCountNoBorders.y - 1,
        baseX1 + (ptsCountNoBorders.y * 2) - 1,
        baseY1 + (ptsCountNoBorders.x * 2) - 1);
      add(
        ptsCountNoBorders.x * ptsCountNoBorders.y - 1,
        baseX1 + (ptsCountNoBorders.y * 2) - 2,
        baseX1 + (ptsCountNoBorders.y * 2) - 1);
    }

  } // anonym NS


  std::unique_ptr<IShape3d> IShape3d::plane(
    const Sdk::Size2I& i_ptsCount, const float i_dist, const float i_textureCoef)
  {
    auto verts = generatePlaneVerts(i_ptsCount, i_dist, i_textureCoef);
    auto inds = generatePlaneInds(i_ptsCount);
    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }


  std::unique_ptr<IShape3d> IShape3d::planeTesselatedBorder(
    const Sdk::Size2I& i_ptsCount, const float i_dist, const float i_textureCoef)
  {
    CONTRACT_EXPECT(i_ptsCount.x > 2);
    CONTRACT_EXPECT(i_ptsCount.y > 2);
    CONTRACT_EXPECT(i_dist > 0);

    // Generate internal low-polygon verts
    const auto ptsCountNoBorders = i_ptsCount - Sdk::Size2I{ 2, 2 };
    auto verts = generatePlaneVerts(ptsCountNoBorders, i_dist, i_textureCoef);
    translateVerts(verts, { i_dist, 0, i_dist });
    const int base = (int)verts.size();


    // Generate border hi-polygon verts
    const auto vertsBorder = generateRectVerts(
      i_ptsCount * 2 - Sdk::Size2I{ 1, 1 }, i_dist / 2, i_textureCoef);
    verts.insert(verts.end(), vertsBorder.begin(), vertsBorder.end());

    // Generate internal inds
    auto inds = generatePlaneInds(ptsCountNoBorders);
    
    // Add indices for border
    addBorderInds(inds, i_ptsCount, base);

    return std::make_unique<Shape3d>(std::move(verts), std::move(inds));
  }

} // ns Dx
