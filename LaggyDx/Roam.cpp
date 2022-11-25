#include "stdafx.h"
#include "Roam.h"

#include <LaggySdk/VectorUtils.h>


namespace Dx
{
  struct Tri
  {
    Tri(const int i_ind1, const int i_ind2, const int i_ind3)
      : ind1(i_ind1)
      , ind2(i_ind2)
      , ind3(i_ind3)
    {
    }

    const int ind1 = 0;
    const int ind2 = 0;
    const int ind3 = 0;

    std::shared_ptr<Tri> parent;
    std::shared_ptr<Tri> baseNeighbor;
    std::shared_ptr<Tri> leftChild;
    std::shared_ptr<Tri> rightChild;
    std::shared_ptr<Tri> leftNeighbor;
    std::shared_ptr<Tri> rightNeighbor;

    int depth() const
    {
      if (parent)
        return parent->depth() + 1;
      return 1;
    }
  };


  Roam::Roam(const double i_size)
  {
    d_points.push_back(VertexPosNormText::pos({ 0, 0, 0 }));
    d_points.push_back(VertexPosNormText::pos({ 0, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, (float)i_size }));
    d_points.push_back(VertexPosNormText::pos({ (float)i_size, 0, 0 }));

    tri1 = std::make_shared<Tri>(0, 1, 3);
    tri2 = std::make_shared<Tri>(2, 3, 1);

    tri1->baseNeighbor = tri2;
    tri2->baseNeighbor = tri1;

    divideTri(tri1);
    auto triLeft = tri1->leftChild;
    auto triRight = tri1->rightChild;
    for (int i = 0; i < 35; ++i)
    {
      divideTri(triLeft);
      divideTri(triRight);
      triLeft = triLeft->leftChild;
      triRight = triRight->rightChild;
    }

    setNormalsAndTexCoords();
    collectInds();
  }


  void Roam::divideTri(std::shared_ptr<Tri> i_tri)
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
      divideTri(i_tri->baseNeighbor);

    auto newPointPos = (d_points.at(i_tri->ind2).position + d_points.at(i_tri->ind3).position) / 2;
    d_points.push_back(VertexPosNormText::pos(std::move(newPointPos)));
    const int newInd = (int)d_points.size() - 1;

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
        divideTri(i_tri->baseNeighbor);
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
      p.texture = Sdk::Vector2F{ p.position.x, p.position.z };
      p.normal = { 0, 1, 0 };
    }
  }

  void Roam::collectInds()
  {
    d_inds = collectInds(*tri1);
    Sdk::mergeVectors(d_inds, collectInds(*tri2));
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
