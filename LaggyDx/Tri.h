#pragma once


namespace Dx
{
  class Tri
  {
  public:
    Tri(const int i_ind1, const int i_ind2, const int i_ind3);

    const int ind1 = 0;
    const int ind2 = 0;
    const int ind3 = 0;

    std::shared_ptr<Tri> parent;
    std::shared_ptr<Tri> baseNeighbor;
    std::shared_ptr<Tri> leftChild;
    std::shared_ptr<Tri> rightChild;
    std::shared_ptr<Tri> leftNeighbor;
    std::shared_ptr<Tri> rightNeighbor;

    int depth() const;
  };

} // ns Dx
