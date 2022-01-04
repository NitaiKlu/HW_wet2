#ifndef RTREE_H_
#define RTREE_H_

#include "Level.h"
#include "Tree.h"
#include "RNode.h"

template <class T>
class RTree : public Tree<T>
{
private:
    int _rank_size;
    RNode<T> *RR(RNode<T> *not_balanced);
    RNode<T> *LL(RNode<T> *not_balanced);
    RNode<T> *rotate(RNode<T> *not_balanced);
    /**
     * rotate - should have updateWeight whenever a RNode has a new son
     * all changes of polimorphism shit?
     * internal insert (and elsewhere if needed) change TNode c'tor to RNode c'tor
     * add Rank(key, index): returns the indexish rank of key
     * add Search(rank, index): return the key with the indexish rank (except for the last 2 ranks in the array)
     *
     * */

public:
    RTree(int rank_size);
    RTree(const RTree &copy);
    ~RTree();
    int Rank(const int key, const int ind) const;
    int Search(const int rank, const int ind) const;
};

template <class T>
RTree<T>::RTree(int rank_size)
    : Tree<T>(), _rank_size(rank_size)
{
}

template <class T>
RTree<T>::RTree(const RTree &copy)
    : Tree<T>(copy), _rank_size(copy._rank_size)
{
}

template <class T>
RNode<T> *RTree<T>::RR(RNode<T> *not_balanced)
{
    RNode<T> *rotated = Tree<T>::RR(not_balanced);
    rotated->updateWeight();
    return rotated;
}

template <class T>
RNode<T> *RTree<T>::LL(RNode<T> *not_balanced)
{
    RNode<T> *rotated = Tree<T>::LL(not_balanced);
    rotated->updateWeight();
    return rotated;
}

template <class T>
RNode<T> *RTree<T>::rotate(RNode<T> *not_balanced)
{
    if (not_balanced == nullptr)
        return nullptr;
    not_balanced->updateWeight();
    return Tree<T>::rotate(not_balanced);
}

template <class T>
int RTree<T>::Rank(const int key, const int ind) const
{
    return 0;
}

template <class T>
int RTree<T>::Search(const int rank, const int ind) const
{
    return 0;
}
#endif // RTREE_H_