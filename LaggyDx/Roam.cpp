#include "stdafx.h"
#include "Roam.h"

#include "HeightMap.h"
#include "Tri.h"

#include <LaggySdk/VectorUtils.h>


namespace Dx
{
  Roam::Roam(const double i_size)
  {
    d_points.push_back(VertexPosNormText::pos({ 0, 0, 0 }));
    d_points.push_back(VertexPosNormText::pos({ 0, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, 0 }));

    d_root = std::make_shared<Tri>(0, 1, 3);
    auto rootBase = std::make_shared<Tri>(2, 3, 1);

    d_root->baseNeighbor = rootBase;
    rootBase->baseNeighbor = d_root;
  }

  Roam::Roam(const double i_size, DividerPredicate i_pred)
    : Roam(i_size)
  {
    tesselate(i_pred);

    setNormalsAndTexCoords();
    collectInds();
  }

  Roam::Roam(const HeightMap& i_heightMap, const double i_precision)
    : Roam(i_heightMap.getWidth() - 1)
  {
    CONTRACT_EXPECT(i_heightMap.getWidth() == i_heightMap.getHeight());

    // Move initials points to the correct heights
    for (auto& point : d_points)
      point.position.y = (float)i_heightMap.getHeight(point.position.x, point.position.z);

    tesselate(i_heightMap, i_precision);

    setNormalsAndTexCoords();
    collectInds();
  }


  void Roam::tesselate(DividerPredicate i_pred)
  {
    CONTRACT_EXPECT(d_root);
    CONTRACT_EXPECT(d_root->baseNeighbor);

    tesselate(d_root, i_pred);
    tesselate(d_root->baseNeighbor, i_pred);
  }

  void Roam::tesselate(std::shared_ptr<Tri> i_tri, DividerPredicate i_pred)
  {
    CONTRACT_EXPECT(i_tri);

    if (!i_pred(*i_tri, d_points))
      return;

    divideTri(i_tri);

    tesselate(i_tri->leftChild, i_pred);
    tesselate(i_tri->rightChild, i_pred);
  }


  void Roam::tesselate(const HeightMap& i_heightMap, const double i_precision)
  {
    CONTRACT_EXPECT(d_root);
    CONTRACT_EXPECT(d_root->baseNeighbor);

    tesselate(d_root, i_heightMap, i_precision);
    tesselate(d_root->baseNeighbor, i_heightMap, i_precision);
  }


  void Roam::tesselate(std::shared_ptr<Tri> i_tri, const HeightMap& i_heightMap, const double i_precision)
  {
    CONTRACT_EXPECT(i_tri);

    const auto testPoint = (
      d_points.at(i_tri->ind2).position +
      d_points.at(i_tri->ind3).position) / 2;
    const double refHeight = i_heightMap.getHeight(testPoint.x, testPoint.z);
    const double heightDiff = std::abs(testPoint.y - refHeight);

    const int depth = i_tri->depth();
    constexpr int minDepth = 10;
    constexpr int maxDepth = 20;
    if (
      (depth >= minDepth) &&
      (heightDiff < i_precision || depth >= maxDepth))
    {
      return;
    }

    const int newInd = divideTri(i_tri);
    d_points.at(newInd).position.y = (float)refHeight;

    tesselate(i_tri->leftChild, i_heightMap, i_precision);
    tesselate(i_tri->rightChild, i_heightMap, i_precision);
  }


  int Roam::divideTri(std::shared_ptr<Tri> i_tri, std::optional<int> i_newPointInd)
  {
    CONTRACT_EXPECT(i_tri);

    if (i_tri->leftChild && i_tri->rightChild)
    {
      // Tri is already divided
      return i_tri->leftChild->ind1;
    }
    CONTRACT_EXPECT(!i_tri->leftChild);
    CONTRACT_EXPECT(!i_tri->rightChild);

    if (i_tri->baseNeighbor && i_tri->baseNeighbor->baseNeighbor != i_tri)
      divideTri(i_tri->baseNeighbor);
    
    if (!i_newPointInd)
    {
      auto newPointPos = (d_points.at(i_tri->ind2).position + d_points.at(i_tri->ind3).position) / 2;
      d_points.push_back(VertexPosNormText::pos(std::move(newPointPos)));
      i_newPointInd = (int)d_points.size() - 1;
    }

    i_tri->leftChild = std::make_shared<Tri>(*i_newPointInd, i_tri->ind1, i_tri->ind2);
    i_tri->rightChild = std::make_shared<Tri>(*i_newPointInd, i_tri->ind3, i_tri->ind1);

    i_tri->leftChild->parent = i_tri;
    i_tri->rightChild->parent = i_tri;

    i_tri->leftChild->baseNeighbor = i_tri->leftNeighbor;
    i_tri->leftChild->leftNeighbor = i_tri->rightChild;

    i_tri->rightChild->baseNeighbor = i_tri->rightNeighbor;
    i_tri->rightChild->rightNeighbor = i_tri->leftChild;

    // Link left neighbor to the new children
    if (i_tri->leftNeighbor)
    {
      if (i_tri->leftNeighbor->baseNeighbor == i_tri)
        i_tri->leftNeighbor->baseNeighbor = i_tri->leftChild;
      else if (i_tri->leftNeighbor->leftNeighbor == i_tri)
        i_tri->leftNeighbor->leftNeighbor = i_tri->leftChild;
      else if (i_tri->leftNeighbor->rightNeighbor == i_tri)
        i_tri->leftNeighbor->rightNeighbor = i_tri->leftChild;
    }

    // Link right neighbor to the new children
    if (i_tri->rightNeighbor)
    {
      if (i_tri->rightNeighbor->baseNeighbor == i_tri)
        i_tri->rightNeighbor->baseNeighbor = i_tri->rightChild;
      else if (i_tri->rightNeighbor->rightNeighbor == i_tri)
        i_tri->rightNeighbor->rightNeighbor = i_tri->rightChild;
      else if (i_tri->rightNeighbor->leftNeighbor == i_tri)
        i_tri->rightNeighbor->leftNeighbor = i_tri->rightChild;
    }

    // Link base neighbor to the new children
    if (i_tri->baseNeighbor)
    {
      if (i_tri->baseNeighbor->leftChild)
      {
        i_tri->baseNeighbor->leftChild->rightNeighbor = i_tri->rightChild;
        i_tri->baseNeighbor->rightChild->leftNeighbor = i_tri->leftChild;
        i_tri->leftChild->rightNeighbor = i_tri->baseNeighbor->rightChild;
        i_tri->rightChild->leftNeighbor = i_tri->baseNeighbor->leftChild;
      }
      else
      {
        divideTri(i_tri->baseNeighbor, *i_newPointInd);
      }
    }
    else
    {
      // Edge triangle
      i_tri->leftChild->rightNeighbor = NULL;
      i_tri->rightChild->leftNeighbor = NULL;
    }

    return *i_newPointInd;
  }


  void Roam::setNormalsAndTexCoords()
  {
    for (auto& p : d_points)
    {
      p.texture = Sdk::Vector2F{ p.position.x, p.position.z };
      p.normal = { 0, 1, 0 };
    }
  }

  void Roam::collectInds()
  {
    CONTRACT_EXPECT(d_root);
    CONTRACT_EXPECT(d_root->baseNeighbor);

    d_inds = collectInds(*d_root);
    Sdk::mergeVectors(d_inds, collectInds(*d_root->baseNeighbor));
  }

  std::vector<int> Roam::collectInds(const Tri& i_tri)
  {
    std::vector<int> inds;

    if (i_tri.leftChild)
      Sdk::mergeVectors(inds, collectInds(*i_tri.leftChild));
    if (i_tri.rightChild)
      Sdk::mergeVectors(inds, collectInds(*i_tri.rightChild));

    if (!i_tri.leftChild && !i_tri.rightChild)
      inds = { i_tri.ind1, i_tri.ind2, i_tri.ind3 };

    return inds;
  }


  const std::vector<VertexPosNormText>& Roam::getPoints() const
  {
    return d_points;
  }

  const std::vector<int>& Roam::getInds() const
  {
    return d_inds;
  }

} // Dx
