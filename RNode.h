#ifndef RNODE_H_
#define RNODE_H_

#include "Level.h"
#include "TNode.h"

template <class T>
class RNode: public TNode<T>
{
private:
    int size;
    int* weight;

public:
    RNode(int size);
    RNode(int key, const T &data, int size, RNode *parent = nullptr);
    RNode(const RNode &copy) = delete;
    ~RNode();
    /**
     * changes in functions if necessary
     * */
    int getWeight(int i) const;
    void updateWeight(); //update the rank array
};

template <class T>
RNode<T>::RNode(int size)
    : TNode<T>(), size(size)
{
    weight = new int[size];
}

template <class T>
RNode<T>::RNode(int key, const T &data, int size, RNode<T> *parent)
    : TNode<T>(key, data, parent), size(size)
{
}

template <class T>
RNode<T>::~RNode()
{
    delete[] weight;
}

template <class T>
int RNode<T>::getWeight(int i) const
{
    if (i>=0 && i<size+2)
    {
        return weight[i];
    }
    return -1;
}

template <class T>
void RNode<T>::updateWeight()
{
    for (int i = 0; i < size+2; i++)
    {
        int l_weight = (this->left == nullptr) ? -1 : this->left->weight[i];
        int r_weight = (this->right == nullptr) ? -1 : this->right->weight[i];
        weight[i] = std::max(l_weight, r_weight) + 1;
    }
}

#endif //RNODE_H_