#ifndef RTREE_H_
#define RTREE_H_

#include <iostream>
#include "RNode.h"
#include <cassert>
#include <memory>

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
    RNode<T> *internalUpdateSize(RNode<T> *node, int key_to_update, int index, int new_size);
    int internalRank(RNode<T> *node, int key_to_find) const;
    RNode<T> *internalInsert(RNode<T> *node, int key_to_insert, const T &data, RNode<T> *to_return);
    RNode<T> *internalRemove(RNode<T> *node, int key_to_remove);
    RNode<T> *internalClear(RNode<T> *root_ptr);

public:
    RTree(int rank_size);
    RTree(const RTree<T> &copy) = delete;
    ~RTree();
    /*
    class const_iterator;
    const_iterator begin() const;
    const_iterator reverseBegin() const;
    const_iterator end() const;
    const_iterator search(const int key) const;
    void ArrayToTree(T *array, int *keys, int start, int end);
    void printTree() const;
    void printTree(const std::string &prefix, const RNode<T> *node, bool isLeft) const;
    */
    RNode<T> *search(const int key) const;
    bool isEmpty() const;
    int getSize() const;
    bool isExist(int key_to_find) const;
    T &getData(int key_to_find) const;
    T &insert(int key, const T &data);
    void remove(int key);
    void clearAll();
    int Rank(const int key, const int ind) const;
    int Select(const int rank, const int ind) const;
    void updateSize(int key_to_update, int index, int new_size);
};
/*
//This iterator is calld const because the TNodes' keys are immutable
template <class T>
class Tree<T>::const_iterator
{
private:
    const Tree<T> *tree;
    RNode<T> *element;
    const_iterator(const Tree<T> *tree, RNode<T> *element)
        : tree(tree), element(element) {}
    friend class Tree<T>;
    RNode<T> *getNode() const;

public:
    const_iterator(const const_iterator &copy) = default;
    ~const_iterator() = default;
    Tree<T>::const_iterator &operator=(const const_iterator &it) = default;

    T &getData() const; // allowing the user to change the DATA but not the key! - only through using iterator
    const int getKey() const;

    bool operator==(const const_iterator &it) const;
    bool operator!=(const const_iterator &it) const;

    Tree<T>::const_iterator &operator++();
    Tree<T>::const_iterator &operator--();
};
////////////////////////IMPLEMENTATION///////////////////////

//////// const_iterator ////////

template <class T>
typename Tree<T>::const_iterator &Tree<T>::const_iterator ::operator++()
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
typename Tree<T>::const_iterator &Tree<T>::const_iterator ::operator--()
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

bool Tree<T>::const_iterator ::operator==(const const_iterator &it) const
{
    if (this->element == it.element)
    {
        return true;
    }
    return false;
}

template <class T>
bool Tree<T>::const_iterator ::operator!=(const const_iterator &it) const
{
    return !(*this == it);
}

template <class T>
T &Tree<T>::const_iterator ::getData() const
{
    if (this->element == nullptr)
    {
        throw std::invalid_argument( "Null arg" );
    }
    return this->element->getData();
}

template <class T>
const int Tree<T>::const_iterator ::getKey() const
{
    if (this->element == nullptr)
    {
        throw std::invalid_argument( "Null arg" );
    }

    return this->element->getKey();
}

template <class T>
RNode<T> *Tree<T>::const_iterator ::getNode() const
{
    return this->element;
}
*/
//////// Tree ////////
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
    root = internalClear(root);
    left_most = nullptr;
    right_most = nullptr;
}

template <class T>
bool RTree<T>::isEmpty() const
{
    return root == nullptr;
}

template <class T>
int RTree<T>::getSize() const
{
    return size;
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
RNode<T> *RTree<T>::internalUpdateSize(RNode<T> *node, int key_to_update, int index, int new_size)
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    RNode<T> *search;
    if (key_to_update == key)
    {
        search = node;
        search->updateSize(index, new_size);
    }

    else if (key_to_update > key)
    {
        search = internalSearch(node->getRight(), key_to_update);
    }
    else
    {
        search = internalSearch(node->getLeft(), key_to_update);
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

template <class T>
bool RTree<T>::isExist(int key_to_find) const
{
    return internalSearch(root, key_to_find) != nullptr;
}

template <class T>
T &RTree<T>::getData(int key_to_find) const
{
    return search(key_to_find).getData();
}


template <class T>
RNode<T> * RTree<T>::search(const int key) const
{
    return internalSearch(root, key);
}

/*
template <class T>
typename Tree<T>::const_iterator Tree<T>::search(const int key) const
{
    return Tree<T>::const_iterator(this, internalSearch(root, key));
}
*/

template <class T>
RNode<T> *RTree<T>::internalInsert(RNode<T> *node, int key_to_insert, const T &data, RNode<T> *to_return)
{
    if (node == nullptr)
    {
        to_return = new RNode<T>(key_to_insert, data);
        size++;
        return to_return;
    }
    else
    {
        int key = node->getKey();
        if (key_to_insert == key)
        {
            // We chose to enable information updating//
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
            node->setRight(internalRemove(node->getRight(), next_node->getKey()));
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
}
#endif // RTREE_H_