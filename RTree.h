#ifndef RTREE_H_
#define RTREE_H_

#include <iostream>
#include "RNode.h"
#include <cassert>
#include <memory>
#include <math.h>
using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

// Each node in RTree holds a _rank_size sized array,
// whereas node.weight[i] is the sum of weight[i] under that node.

// We define rank(ind) of a node- "n" as the sum of all weight[ind] of
// nodes before (smaller than) "n", including "n" itself.

// Rank(key, ind) returns the rank(ind) of the node with key=key.
// Select(rank, ind) returns the key of the node with rank(ind)=rank.
template <class T>
class RTree
{
private:
    int rank_size;
    RNode<T> *root;
    RNode<T> *left_most;
    RNode<T> *right_most;
    int size;
    RNode<T> *rotate(RNode<T> *not_balanced);
    RNode<T> *RR(RNode<T> *not_balanced);
    RNode<T> *LL(RNode<T> *not_balanced);
    RNode<T> *RL(RNode<T> *not_balanced);
    RNode<T> *LR(RNode<T> *not_balanced);
    /*
    RNode<T> *next_bigger(RNode<T> *vertice) const;
    RNode<T> *next_smaller(RNode<T> *vertice) const;
    RNode<T> *internalArrayToTree(RNode<T> *parent, int *keys, T *array, int start, int end);
    */
    RNode<T> *internalSearch(RNode<T> *node, int key_to_find) const;
    // RNode<T> *internalUpdateSize(RNode<T> *node, int key_to_update, int index, int new_size);
    RNode<T> *internalAddToSize(RNode<T> *node, int key_to_update, int index, int addition);
    //int internalRank(RNode<T> *node, int key_to_find) const;
    RNode<T> *internalInsert(RNode<T> *node, int key_to_insert, const T &data, RNode<T> *to_return);
    RNode<T> *internalRemove(RNode<T> *node, int key_to_remove);
    RNode<T> *internalClear(RNode<T> *root_ptr);
    RNode<T> *search(const int key) const;
    RNode<T> *internalSearchFromBelow(RNode<T> *node, int upper_key) const;
    RNode<T> *internalSearchFromAbove(RNode<T> *node, int lower_key) const;
    int internalRank(RNode<T> *node, int key, int ind, int r) const;
    RNode<T> *internalSelect(RNode<T> *node, int rank, int ind) const;
    RNode<T> *internalSelectFromBelow(RNode<T> *node, int upper_key, int rank) const;
    RNode<T> *internalSelectFromAbove(RNode<T> *node, int rank, int ind, RNode<T> *keeper, int min_diff) const;
    RNode<T> *internalArrayToTree(RNode<T> *parent, RNode<T> **array, int start, int end);
    RNode<T> *next_bigger(RNode<T> *vertice) const;
    RNode<T> *next_smaller(RNode<T> *vertice) const;
    void overwriteSizes(RNode<T> *target, const RNode<T> *source);

public:
    RTree(int rank_size);
    RTree(const RTree<T> &copy) = delete;
    ~RTree();
    class const_iterator;
    const_iterator begin() const;
    const_iterator reverseBegin() const;
    const_iterator end() const;
    //const_iterator search(const int key) const;
    void printTree() const;
    void printTree(const std::string &prefix, const RNode<T> *node, bool isLeft) const;
    void ArrayToTree(RNode<T> **array, int start, int end);
    bool isEmpty() const;
    int getTreeSize() const;
    int getHighestLevel() const;
    bool isExist(int key_to_find) const;
    T &getData(int key_to_find) const;
    T &insert(int key, const T &data);
    void remove(int key);
    void clearAll();

    void checkBounds(int index) const;

    int getSizeAt(int key, int ind) const;

    int getWeight(int key) const;
    int getWeightAt(int key, int ind) const;

    int rank(int key) const;
    int rankAt(int key, int ind) const;

    int select(int rank) const;
    int selectAt(int rank, int ind) const; // Might not be well defined

    int selectFromBelow(int upper_key, int rank) const;
    int selectFromBelowAt(int upper_key, int rank, int ind) const; // Might not be well defined

    int selectFromAbove(int lower_key, int rank) const;
    int selectFromAboveAt(int rank, int ind) const; // Might not be well defined

    void updateSize(int key_to_update, int index, int new_size);
    void addToSize(int key_to_update, int index, int addition);
    // these two return a key
    int searchFromBelow(int upper_key) const;
    int searchFromAbove(int lower_key) const;

    void printNodeRank(int key, int index) const;

};

//This iterator is calld const because the TNodes' keys are immutable
template <class T>
class RTree<T>::const_iterator
{
private:
    const RTree<T> *tree;
    RNode<T> *element;
    const_iterator(const RTree<T> *tree, RNode<T> *element)
        : tree(tree), element(element) {}
    friend class RTree<T>;

public:
    const_iterator(const const_iterator &copy) = default;
    ~const_iterator() = default;
    RTree<T>::const_iterator &operator=(const const_iterator &it) = default;

    T &getData() const; // allowing the user to change the DATA but not the key! - only through using iterator
    const int getKey() const;
    RNode<T> *getNode() const;
    bool operator==(const const_iterator &it) const;
    bool operator!=(const const_iterator &it) const;

    RTree<T>::const_iterator &operator++();
    RTree<T>::const_iterator &operator--();
};
////////////////////////IMPLEMENTATION///////////////////////

//////// const_iterator ////////

template <class T>
typename RTree<T>::const_iterator &RTree<T>::const_iterator ::operator++()
{
    if (*this == tree->reverseBegin()) // no where to continue, biggest node
    {
        this->element = nullptr; // now it is end()
        return *this;
    }
    if (this->element->getParent() == nullptr && this->element->getRight() == nullptr)
    {
        this->element = nullptr; // now it is end()
        return *this;
    }
    else if (this->element->getRight() != nullptr)
    { // the next bigger is in the right sub-tree
        this->element = this->element->getRight()->getMin();
    }
    else // next big one is somewhere above
    {
        this->element = tree->next_bigger(this->element);
    }
    return *this;
}

template <class T>
typename RTree<T>::const_iterator &RTree<T>::const_iterator ::operator--()
{
    if (*this == tree->begin())
    {
        this->element = nullptr;
    }
    else if (this->element->getLeft() != nullptr)
    { // the next smaller one is the left son
        this->element = this->element->getLeft()->getMax();
    }
    else // next small one is somewhere above
    {
        this->element = tree->next_smaller(this->element);
    }
    return *this;
}
template <class T>

bool RTree<T>::const_iterator ::operator==(const const_iterator &it) const
{
    if (this->element == it.element)
    {
        return true;
    }
    return false;
}

template <class T>
bool RTree<T>::const_iterator ::operator!=(const const_iterator &it) const
{
    return !(*this == it);
}

template <class T>
T &RTree<T>::const_iterator ::getData() const
{
    if (this->element == nullptr)
    {
        throw std::invalid_argument( "Null arg" );
    }
    return this->element->getData();
}

template <class T>
const int RTree<T>::const_iterator ::getKey() const
{
    if (this->element == nullptr)
    {
        throw std::invalid_argument( "Null arg" );
    }

    return this->element->getKey();
}

template <class T>
RNode<T> *RTree<T>::const_iterator ::getNode() const
{
    return this->element;
}

//////// Tree ////////
template <class T>
typename RTree<T>::const_iterator RTree<T>::begin() const
{
    return RTree<T>::const_iterator(this, left_most);
}
template <class T>
typename RTree<T>::const_iterator RTree<T>::reverseBegin() const
{
    return RTree<T>::const_iterator(this, right_most);
}
template <class T>
typename RTree<T>::const_iterator RTree<T>::end() const
{
    return RTree<T>::const_iterator(this, nullptr);
}


template <class T>
void RTree<T>::ArrayToTree(RNode<T> **array, int start, int end)
{
    root = internalArrayToTree(nullptr, array, start, end - 1);
    if (root == nullptr)
    {
        return;
    }
    left_most = root->getMin();
    right_most = root->getMax();
}

template <class T>
RNode<T>* RTree<T>::internalArrayToTree(RNode<T> *parent, RNode<T> **array, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int middle = (start + end) / 2;
    RNode<T> *to_replace = array[middle];
    RNode<T> *curr = new RNode<T>(rank_size ,to_replace->getKey(), to_replace->getData());
    for (int i = 1; i <= rank_size; i++)
    {
        curr->addToSize(i, to_replace->getSizeAt(i));
    }
    size++;
    curr->setLeft(internalArrayToTree(curr, array, start, middle - 1));
    curr->setRight(internalArrayToTree(curr, array, middle + 1, end));
    curr->updateAllWeights();
    return curr;
}

template <class T>
RNode<T> *RTree<T>::next_bigger(RNode<T> *vertice) const
{
    RNode<T> *parent = vertice->getParent();
    if (parent->getLeft() == vertice)
    { // vertice is smaller than his parent
        return parent;
    }
    return next_bigger(parent);
}

template <class T>
RNode<T> *RTree<T>::next_smaller(RNode<T> *vertice) const
{
    RNode<T> *parent = vertice->getParent();
    if (parent->getRight() == vertice)
    { // vertice is bigger than his parent
        return parent;
    }
    return next_smaller(parent);
}


template <class T>
RTree<T>::RTree(int rank_size) : rank_size(rank_size), root(nullptr), left_most(nullptr), right_most(nullptr), size(0) {}

/*
template <class T>
RTree<T>::RTree(const RTree<T> &copy)
{
    for (RTree<T>::const_iterator it = copy.begin(); it != copy.end(); ++it)
    {
        insert(it.getKey(), it.getData());
    }
}
*/

template <class T>
RNode<T> *RTree<T>::internalClear(RNode<T> *root_ptr)
{
    if (root_ptr != nullptr)
    {
        root_ptr->setRight(internalClear(root_ptr->getRight()));
        root_ptr->setLeft(internalClear(root_ptr->getLeft()));
        delete root_ptr;
        size--;
    }
    return nullptr;
}

template <class T>
RTree<T>::~RTree()
{
    clearAll();
}

template <class T>
bool RTree<T>::isEmpty() const
{
    return root == nullptr;
}

template <class T>
int RTree<T>::getTreeSize() const
{
    return size;
}

template <class T>
int RTree<T>::getHighestLevel() const
{
    return right_most->getKey();
}

template <class T>
RNode<T> *RTree<T>::LL(RNode<T> *not_balanced)
{
    // helpful definitions:
    RNode<T> *parent = not_balanced->getParent();
    RNode<T> *A = not_balanced->getLeft();
    RNode<T> *temp = A->getRight();
    // rotation:
    not_balanced->setLeft(temp);
    if (temp != nullptr)
    {
        temp->setParent(not_balanced);
    }
    A->setRight(not_balanced);
    not_balanced->setParent(A);
    // resetting the original parent of not_balanced
    if (parent != nullptr)
    {
        if (parent->getLeft() == not_balanced)
        { // not_balanced is the left son
            parent->setLeft(A);
        }
        else
        { // not_balanced is the right son
            parent->setRight(A);
        }
    }

    A->setParent(parent);
    not_balanced->updateHeight();
    not_balanced->updateBalance();
    not_balanced->updateAllWeights();
    A->updateHeight();
    A->updateBalance();
    A->updateAllWeights();
    return A;
}

template <class T>
RNode<T> *RTree<T>::RR(RNode<T> *not_balanced)
{
    // helpful definitions:
    RNode<T> *parent = not_balanced->getParent();
    RNode<T> *A = not_balanced->getRight();
    RNode<T> *temp = A->getLeft();
    // rotation:
    not_balanced->setRight(temp);
    if (temp != nullptr)
    {
        temp->setParent(not_balanced);
    }
    A->setLeft(not_balanced);
    not_balanced->setParent(A);
    // resetting the original parent of not_balanced
    if (parent != nullptr)
    {
        if (parent->getLeft() == not_balanced)
        { // not_balanced is the left son
            parent->setLeft(A);
        }
        else
        { // not_balanced is the right son
            parent->setRight(A);
        }
    }
    A->setParent(parent);
    not_balanced->updateHeight();
    not_balanced->updateBalance();
    not_balanced->updateAllWeights();
    A->updateHeight();
    A->updateBalance();
    A->updateAllWeights();
    return A;
}

template <class T>
RNode<T> *RTree<T>::LR(RNode<T> *not_balanced)
{
    RNode<T> *left_son = not_balanced->getLeft();
    RR(left_son);
    return LL(not_balanced);
}

template <class T>
RNode<T> *RTree<T>::RL(RNode<T> *not_balanced)
{
    RNode<T> *right_son = not_balanced->getRight();
    LL(right_son);
    return RR(not_balanced);
}

/*
template <class T>
RNode<T> *Tree<T>::next_smaller(RNode<T> *vertice) const
{
    RNode<T> *parent = vertice->getParent();
    if (parent->getRight() == vertice)
    { // vertice is bigger than his parent
        return parent;
    }
    return next_smaller(parent);
}

template <class T>
RNode<T> *Tree<T>::next_bigger(RNode<T> *vertice) const
{
    RNode<T> *parent = vertice->getParent();
    if (parent->getLeft() == vertice)
    { // vertice is smaller than his parent
        return parent;
    }
    return next_bigger(parent);
}
*/

template <class T>
RNode<T> *RTree<T>::rotate(RNode<T> *not_balanced)
{
    if (not_balanced == nullptr)
        return nullptr;
    not_balanced->updateHeight();
    not_balanced->updateBalance();
    not_balanced->updateAllWeights();
    if (not_balanced->getBalance() == 2)
    {
        if (not_balanced->getLeft()->getBalance() == -1)
        {
            return LR(not_balanced);
        }
        else
            return LL(not_balanced);
    }
    else if (not_balanced->getBalance() == -2)
    {
        if (not_balanced->getRight()->getBalance() == 1)
        {
            return RL(not_balanced);
        }
        else
            return RR(not_balanced);
    }
    return not_balanced;
}

template <class T>
RNode<T> *RTree<T>::internalSearch(RNode<T> *node, int key_to_find) const
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    if (key_to_find == key)
        return node;
    else if (key_to_find > key)
    {
        return internalSearch(node->getRight(), key_to_find);
    }
    else
    {
        return internalSearch(node->getLeft(), key_to_find);
    }
}

template <class T>
void RTree<T>::checkBounds(int index) const
{
    if (index <= 0 || index > rank_size)
        throw std::out_of_range("Out of rank boundaries");
}

/*
template <class T>
RNode<T> *RTree<T>::internalUpdateSize(RNode<T> *node, int key_to_update, int index, int new_size)
{
    if (node == nullptr)
        return nullptr;
    checkBounds(index);
    int key = node->getKey();
    RNode<T> *search;
    if (key_to_update == key)
    {
        search = node;
        search->updateSize(index, new_size);
    }
    else if (key_to_update > key)
    {
        search = internalUpdateSize(node->getRight(), key_to_update, index, new_size);
    }
    else
    {
        search = internalUpdateSize(node->getLeft(), key_to_update, index, new_size);
    }
    if (search != nullptr)
        node->updateAllWeights();
    return search;
}

template <class T>
void RTree<T>::updateSize(int key_to_update, int index, int new_size)
{
    internalUpdateSize(root, key_to_update, index, new_size);
}
*/

template <class T>
RNode<T> *RTree<T>::internalAddToSize(RNode<T> *node, int key_to_update, int index, int addition)
{
    if (node == nullptr)
        return nullptr;
    checkBounds(index);
    int key = node->getKey();
    RNode<T> *search;
    if (key_to_update == key)
    {
        search = node;
        search->addToSize(index, addition);
    }
    else if (key_to_update > key)
    {
        search = internalAddToSize(node->getRight(), key_to_update, index, addition);
    }
    else
    {
        search = internalAddToSize(node->getLeft(), key_to_update, index, addition);
    }
    if (search != nullptr)
        node->updateWeightAt(index);
    return search;
}

template <class T>
void RTree<T>::addToSize(int key_to_update, int index, int addition)
{
    RNode<T> *node = internalSearch(root, key_to_update);
    if (node == nullptr)
        return;
    checkBounds(index);
    internalAddToSize(root, key_to_update, index, addition);
}

template <class T>
bool RTree<T>::isExist(int key_to_find) const
{
    return internalSearch(root, key_to_find) != nullptr;
}

template <class T>
T &RTree<T>::getData(int key_to_find) const
{
    if(key_to_find == left_most->getKey())
    {
        return left_most->getData();
    }
    return search(key_to_find)->getData();
}

template <class T>
RNode<T> *RTree<T>::search(const int key) const
{
    return internalSearch(root, key);
}

template <class T>
RNode<T> *RTree<T>::internalSearchFromBelow(RNode<T> *node, int upper_key) const
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    RNode<T> *tmp;
    if (upper_key == key)
        return node;
    else if (upper_key > key)
    {
        tmp = internalSearchFromBelow(node->getRight(), upper_key);
        return (tmp == nullptr) ? node : tmp;
    }
    else
    {
        tmp = internalSearchFromBelow(node->getLeft(), upper_key);
        return (tmp == nullptr) ? nullptr : tmp;
    }
}

template <class T>
RNode<T> *RTree<T>::internalSearchFromAbove(RNode<T> *node, int lower_key) const
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    RNode<T> *tmp;
    if (lower_key == key)
        return node;
    else if (lower_key > key)
    {
        tmp = internalSearchFromAbove(node->getRight(), lower_key);
        return (tmp == nullptr) ? nullptr : tmp;
    }
    else
    {
        tmp = internalSearchFromAbove(node->getLeft(), lower_key);
        return (tmp == nullptr) ? node : tmp;
    }
}

template <class T>
int RTree<T>::searchFromBelow(int upper_key) const
{
    return internalSearchFromBelow(root, upper_key)->getKey();
}

template <class T>
int RTree<T>::searchFromAbove(int lower_key) const
{
    return internalSearchFromAbove(root, lower_key)->getKey();
}

template <class T>
int RTree<T>::internalRank(RNode<T> *node, int key, int ind, int r) const
{
    if (node == nullptr)
        return 0;
    int curr_key = node->getKey();
    int left_weight = (node->getLeft() == nullptr) ? 0 : node->getLeft()->getWeightAt(ind);
    if (key > curr_key)
    {
        r += left_weight + node->getSizeAt(ind);
        return internalRank(node->getRight(), key, ind, r);
    }
    else if (key == curr_key)
    {
        return r + left_weight + node->getSizeAt(ind);
    }
    else
    {
        return internalRank(node->getLeft(), key, ind, r);
    }
}

template <class T>
int RTree<T>::rankAt(int key, int ind) const
{
    checkBounds(ind);
    return internalRank(root, key, ind, 0);
}

template <class T>
int RTree<T>::rank(int key) const
{
    return internalRank(root, key, 0, 0);
}

template <class T>
RNode<T> *RTree<T>::internalSelect(RNode<T> *node, int rank, int ind) const
{
    if (node == nullptr)
        return nullptr;
    int left_weight = (node->getLeft() == nullptr) ? 0 : node->getLeft()->getWeightAt(ind);
    if (left_weight == rank - 1)
        return node;
    else if (left_weight > rank - 1)
    {
        return internalSelect(node->getLeft(), rank, ind);
    }
    else
    {
        rank -= left_weight + node->getSizeAt(ind);
        return internalSelect(node->getRight(), rank, ind);
    }
}

template <class T>
int RTree<T>::selectAt(int rank, int ind) const
{
    checkBounds(ind);
    return internalSelect(root, rank, ind)->getKey();
}

template <class T>
int RTree<T>::select(int rank) const
{
    return internalSelect(root, rank, 0)->getKey();
}

/*
template <class T>
typename Tree<T>::const_iterator Tree<T>::search(const int key) const
{
    return Tree<T>::const_iterator(this, internalSearch(root, key));
}
*/
template <class T>
int RTree<T>::selectFromAboveAt(int rank, int ind) const
{
    return internalSelectFromAbove(root, rank, ind, root, -1)->getKey();
}
template <class T>
RNode<T>* RTree<T>::internalSelectFromAbove(RNode<T> *node, int rank, int ind, RNode<T> *keeper, int min_diff) const
{
    if (node == nullptr)
        return nullptr;
    if(rank < 0)
        return keeper;
    int left_weight = (node->getLeft() == nullptr) ? 0 : node->getLeft()->getWeightAt(ind);
    if (left_weight == rank - 1)
        return node;
    if(left_weight + node->getSizeAt(ind) >= rank) { //cause wer'e searching from above
        if(left_weight + node->getSizeAt(ind) - rank < min_diff || min_diff == -1) { //this is closer than keeper
            keeper = node;
            min_diff = left_weight + node->getSizeAt(ind) - rank;
        }
    }
    if (left_weight > rank - 1)
    {
        return internalSelectFromAbove(node->getLeft(), rank, ind, keeper, min_diff);
    }
    else
    {
        rank -= left_weight + node->getSizeAt(ind);
        return internalSelectFromAbove(node->getRight(), rank, ind, keeper, min_diff);
    }
}



template <class T>
RNode<T> *RTree<T>::internalInsert(RNode<T> *node, int key_to_insert, const T &data, RNode<T> *to_return)
{
    if (node == nullptr)
    {
        to_return = new RNode<T>(rank_size, key_to_insert, data);
        size++;
        return to_return;
    }
    else
    {
        int key = node->getKey();
        if (key_to_insert == key)
        {
            //We chose to enable information updating
            node->setData(data);
        }
        else if (key_to_insert > key)
        {
            RNode<T> *new_right = internalInsert(node->getRight(), key_to_insert, data, to_return);
            new_right->setParent(node);
            node->setRight(new_right);
        }
        else
        {
            RNode<T> *new_left = internalInsert(node->getLeft(), key_to_insert, data, to_return);
            new_left->setParent(node);
            node->setLeft(new_left);
        }
    }
    return rotate(node);
}

template <class T>
T &RTree<T>::insert(int key, const T &data)
{
    RNode<T> *to_return = nullptr;
    root = internalInsert(root, key, data, to_return);
    left_most = root->getMin();
    right_most = root->getMax();
    return to_return->getData();
}

template <class T>
RNode<T> *RTree<T>::internalRemove(RNode<T> *node, int key_to_remove)
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    if (key_to_remove == key)
    {
        if (node->getRight() != nullptr && node->getLeft() != nullptr) // node has 2 sons
        {
            const RNode<T> *next_node = node->getRight()->getMin();
            node->setKey(next_node->getKey());
            node->setData(next_node->getDataConst());
            overwriteSizes(node, next_node);
            node->setRight(internalRemove(node->getRight(), next_node->getKey()));
            node->updateAllWeights();
        }
        else
        {
            RNode<T> *tmpNode = node;
            if (node->getRight() == nullptr)
            {
                node = node->getLeft();
            }
            else
            // if (node->getLeft() == nullptr)
            {
                node = node->getRight();
            }
            if (node != nullptr)
            {
                node->setParent(tmpNode->getParent());
            }
            delete tmpNode;
            size--;
        }
    }
    else if (key_to_remove > key)
    {
        RNode<T> *new_right = internalRemove(node->getRight(), key_to_remove);
        if (new_right != nullptr)
        {
            new_right->setParent(node);
        }
        node->setRight(new_right);
    }
    else
    {
        RNode<T> *new_left = internalRemove(node->getLeft(), key_to_remove);
        if (new_left != nullptr)
        {
            new_left->setParent(node);
        }
        node->setLeft(new_left);
    }
    return rotate(node);
}

template <class T>
void RTree<T>::overwriteSizes(RNode<T> *target, const RNode<T> *source)
{
    for (int i = 1; i <= rank_size; i++)
    {
        target->addToSize(i, source->getSizeAt(i) - target->getSizeAt(i));
    }
}

template <class T>
void RTree<T>::remove(int key)
{
    root = internalRemove(root, key);
    left_most = (root != nullptr) ? root->getMin() : nullptr;
    right_most = (root != nullptr) ? root->getMax() : nullptr;
}

/*
template <class T>
typename Tree<T>::const_iterator Tree<T>::begin() const
{
    return Tree<T>::const_iterator(this, left_most);
}

template <class T>
typename Tree<T>::const_iterator Tree<T>::reverseBegin() const
{
    return Tree<T>::const_iterator(this, right_most);
}

template <class T>
typename Tree<T>::const_iterator Tree<T>::end() const
{
    return Tree<T>::const_iterator(this, nullptr);
}

template <class T>
void Tree<T>::ArrayToTree(T *array, int *keys, int start, int end)
{
    root = internalArrayToTree(nullptr, keys, array, start, end);
    if (root == nullptr)
    {
        return;
    }
    left_most = root->getMin();
    right_most = root->getMax();
}

template <class T>
RNode<T> *Tree<T>::internalArrayToTree(RNode<T> *parent, int *keys, T *array, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int middle = (start + end) / 2;
    RNode<T> *curr = new RNode<T>(keys[middle], array[middle], parent);
    size++;
    curr->setLeft(internalArrayToTree(curr, keys, array, start, middle - 1));
    curr->setRight(internalArrayToTree(curr, keys, array, middle + 1, end));
    return curr;
}

template <class T>
void Tree<T>::printTree() const
{
    printTree("", root, false);
}

template <class T>
void Tree<T>::printTree(const std::string &prefix, const RNode<T> *node, bool isLeft) const
{
    if (node != nullptr)
    {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "'--");

        // print the value of the node
        std::cout << node->getKey() << "," << node->getDataConst() << std::endl;

        // enter the next tree level - left and right branch
        printTree(prefix + (isLeft ? "|   " : "    "), node->getLeft(), true);
        printTree(prefix + (isLeft ? "|   " : "    "), node->getRight(), false);
    }
}
*/

template <class T>
void RTree<T>::clearAll()
{
    root = internalClear(root);
    left_most = nullptr;
    right_most = nullptr;
}

template <class T>
void RTree<T>::printNodeRank(int key, int index) const
{
    RNode<T> *node = internalSearch(root, key);
    if (node == nullptr)
    {
        cout << "Key is not in the tree" << endl;
        return;
    }
    cout << "   , Size: " << node->getSizeAt(index);
    cout << "   , Weight[index]: " << node->getWeightAt(index);
    cout << "   , Rank[index]: " << internalRank(root, key, index, 0) << endl;
}

//////////Available ONLY FOR TESTING!//////////
template <class T>
int RTree<T>::getSizeAt(int key, int ind) const
{
    checkBounds(ind);
    return internalSearch(root, key)->getSizeAt(ind);
}


template <class T>
int RTree<T>::getWeightAt(int key, int ind) const
{
    checkBounds(ind);
    return internalSearch(root, key)->getWeightAt(ind);
}
template <class T>
int RTree<T>::getWeight(int key) const
{
    return internalSearch(root, key)->getWeightAt(0);
}
///////////////////////////////////////////////

#endif // RTREE_H_