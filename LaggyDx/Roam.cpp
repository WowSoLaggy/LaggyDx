#include "stdafx.h"
#include "Roam.h"

#include "HeightMap.h"
#include "Tri.h"

#include <LaggySdk/VectorUtils.h>


namespace Dx
{
  Roam::Roam(const double i_size)
  {
    d_points.push_back(VertexPos3NormText::pos({ 0, 0, 0 }));
    d_points.push_back(VertexPos3NormText::pos({ 0, 0, (float)i_size }));
    d_points.push_back(VertexPos3NormText::pos({ (float)i_size, 0, (float)i_size }));
    d_points.push_back(VertexPos3NormText::pos({ (float)i_size, 0, 0 }));

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

  Roam::Roam(const HeightMap& i_heightMap, HeightPredicate i_pred)
    : Roam(i_heightMap.getWidth() - 1)
  {
    CONTRACT_EXPECT(i_heightMap.getWidth() == i_heightMap.getHeight());

    // Move initials points to the correct heights
    for (auto& point : d_points)
      point.position.y = (float)i_heightMap.getHeight(point.position.x, point.position.z);

    tesselate(i_heightMap, i_pred);

    setNormalsAndTexCoords();
    calculateNormals();
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


  void Roam::tesselate(const HeightMap& i_heightMap, HeightPredicate i_pred)
  {
    CONTRACT_EXPECT(d_root);
    CONTRACT_EXPECT(d_root->baseNeighbor);

    tesselate(d_root, i_heightMap, i_pred);
    tesselate(d_root->baseNeighbor, i_heightMap, i_pred);
  }


  void Roam::tesselate(std::shared_ptr<Tri> i_tri, const HeightMap& i_heightMap, HeightPredicate i_pred)
  {
    CONTRACT_EXPECT(i_tri);
    
    const auto testPoint = (
      d_points.at(i_tri->ind2).position +
      d_points.at(i_tri->ind3).position) / 2;
    const double refHeight = i_heightMap.getHeight(testPoint.x, testPoint.z);
    const double heightDiff = std::abs(testPoint.y - refHeight);

    if (!i_pred(*i_tri, heightDiff))
      return;

    divideTri(i_tri, &i_heightMap);

    tesselate(i_tri->leftChild, i_heightMap, i_pred);
    tesselate(i_tri->rightChild, i_heightMap, i_pred);
  }


  void Roam::divideTri(std::shared_ptr<Tri> i_tri, const HeightMap* i_heightMap)
  {
    CONTRACT_EXPECT(i_tri);

    if (i_tri->leftChild && i_tri->rightChild)
    {
      // Tri is already divided
      return;
    }
    CONTRACT_EXPECT(!i_tri->leftChild);
    CONTRACT_EXPECT(!i_tri->rightChild);

    if (i_tri->baseNeighbor && i_tri->baseNeighbor->baseNeighbor != i_tri)
      divideTri(i_tri->baseNeighbor, i_heightMap);

    // If the base neighbor already split, it created the midpoint vertex on
    // our shared edge — reuse that index instead of pushing a duplicate.
    // Otherwise adjacent triangles end up with two coincident vertices that
    // each get only their own side's face-normal averaged, producing a hard
    // shading seam along every internal edge.
    int newInd;
    if (i_tri->baseNeighbor && i_tri->baseNeighbor->leftChild)
    {
      // Both children of baseNeighbor were created with the midpoint at ind1.
      newInd = i_tri->baseNeighbor->leftChild->ind1;
    }
    else
    {
      auto newPointPos = (d_points.at(i_tri->ind2).position + d_points.at(i_tri->ind3).position) / 2;
      if (i_heightMap)
        newPointPos.y = (float)i_heightMap->getHeight(newPointPos.x, newPointPos.z);
      d_points.push_back(VertexPos3NormText::pos(std::move(newPointPos)));
      newInd = (int)d_points.size() - 1;
    }

    i_tri->leftChild = std::make_shared<Tri>(newInd, i_tri->ind1, i_tri->ind2);
    i_tri->rightChild = std::make_shared<Tri>(newInd, i_tri->ind3, i_tri->ind1);

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
        divideTri(i_tri->baseNeighbor, i_heightMap);
      }
    }
    else
    {
      // Edge triangle
      i_tri->leftChild->rightNeighbor = NULL;
      i_tri->rightChild->leftNeighbor = NULL;
    }
  }


  void Roam::setNormalsAndTexCoords()
  {
    for (auto& p : d_points)
    {
      p.normal = { 0, 1, 0 };
      p.texture = Sdk::Vector2F{ p.position.x, p.position.z };
    }
  }

  void Roam::calculateNormals()
  {
    CONTRACT_EXPECT(d_root);
    CONTRACT_EXPECT(d_root->baseNeighbor);

    std::unordered_map<int, int> addedNormalsNum;
    std::unordered_map<int, Sdk::Vector3F> addedNormals;

    auto getNormal = [&](const Tri& i_tri) {
      const auto sideL = d_points.at(i_tri.ind2).position - d_points.at(i_tri.ind1).position;
      const auto sideR = d_points.at(i_tri.ind3).position - d_points.at(i_tri.ind1).position;
      return Sdk::normalize(Sdk::cross(sideL, sideR));
    };

    std::function<void(const Tri&)> calc = [&](const Tri& i_tri) {
      if (i_tri.leftChild)
        calc(*i_tri.leftChild);
      if (i_tri.rightChild)
        calc(*i_tri.rightChild);

      if (!i_tri.leftChild && !i_tri.rightChild)
      {
        const auto normal = getNormal(i_tri);
        
        addedNormals[i_tri.ind1] += normal;
        addedNormals[i_tri.ind2] += normal;
        addedNormals[i_tri.ind3] += normal;

        ++addedNormalsNum[i_tri.ind1];
        ++addedNormalsNum[i_tri.ind2];
        ++addedNormalsNum[i_tri.ind3];
      }
    };

    calc(*d_root);
    calc(*d_root->baseNeighbor);

    for (int i = 0; i < (int)d_points.size(); ++i)
      d_points[i].normal = Sdk::normalize(addedNormals[i]);
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


  const std::vector<VertexPos3NormText>& Roam::getPoints() const
  {
    return d_points;
  }

  std::vector<VertexPos3NormText>& Roam::getPoints()
  {
    return d_points;
  }

  const std::vector<int>& Roam::getInds() const
  {
    return d_inds;
  }

} // Dx
