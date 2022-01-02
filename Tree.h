#ifndef TREE_H_
#define TREE_H_

#include <iostream>
#include "TNode.h"
#include <cassert>
#include <memory>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

template <class T>
class Tree
{
private:
    TNode<T> *root;
    TNode<T> *left_most;
    TNode<T> *right_most;
    int size;
    TNode<T> *rotate(TNode<T> *not_balanced);
    TNode<T> *RR(TNode<T> *not_balanced);
    TNode<T> *LL(TNode<T> *not_balanced);
    TNode<T> *RL(TNode<T> *not_balanced);
    TNode<T> *LR(TNode<T> *not_balanced);
    TNode<T> *next_bigger(TNode<T> *vertice) const;
    TNode<T> *next_smaller(TNode<T> *vertice) const;
    friend class const_iterator;
    TNode<T> *internalSearch(TNode<T> *node, int key_to_find) const;
    TNode<T> *internalInsert(TNode<T> *node, int key_to_insert, const T &data, TNode<T> *to_return);
    TNode<T> *internalRemove(TNode<T> *node, int key_to_remove);
    TNode<T>* internalClear(TNode<T> *root_ptr);
    TNode<T> *internalArrayToTree(TNode<T> *parent, int *keys, T *array, int start, int end);

public:
    class const_iterator;
    Tree();
    Tree(const Tree<T> &copy);
    ~Tree();
    const_iterator begin() const;
    const_iterator reverseBegin() const;
    const_iterator end() const;
    bool isEmpty() const;
    int getSize() const;
    bool isExist(int key_to_find) const;
    T &getData(int key_to_find) const;
    const_iterator search(const int key) const;
    T &insert(int key, const T &data);
    void remove(int key); // remove a vertice by its key
    void ArrayToTree(T *array, int *keys, int start, int end);
    void printTree() const;
    void printTree(const std::string &prefix, const TNode<T> *node, bool isLeft) const;
    void clearAll();
};

//This iterator is calld const because the TNodes' keys are immutable
template <class T>
class Tree<T>::const_iterator
{
private:
    const Tree<T> *tree;
    TNode<T> *element;
    const_iterator(const Tree<T> *tree, TNode<T> *element)
        : tree(tree), element(element) {}
    friend class Tree<T>;
    TNode<T> *getNode() const;

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
TNode<T> *Tree<T>::const_iterator ::getNode() const
{
    return this->element;
}

//////// Tree ////////
template <class T>
Tree<T>::Tree() : root(nullptr), left_most(nullptr), right_most(nullptr), size(0) {}

template <class T>
Tree<T>::Tree(const Tree<T> &copy)
{
    for (Tree<T>::const_iterator it = copy.begin(); it != copy.end(); ++it)
    {
        insert(it.getKey(), it.getData());
    }
}

template <class T>
TNode<T>* Tree<T>::internalClear(TNode<T> *root_ptr)
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
Tree<T>::~Tree()
{
    root = internalClear(root);
    left_most = nullptr;
    right_most = nullptr;
}

template <class T>
bool Tree<T>::isEmpty() const
{
    return root == nullptr;
}

template <class T>
int Tree<T>::getSize() const
{
    return size;
}

template <class T>
TNode<T> *Tree<T>::LL(TNode<T> *not_balanced)
{
    // helpful definitions:
    TNode<T> *parent = not_balanced->getParent();
    TNode<T> *A = not_balanced->getLeft();
    TNode<T> *temp = A->getRight();
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
    A->updateHeight();
    A->updateBalance();
    return A;
}

template <class T>
TNode<T> *Tree<T>::RR(TNode<T> *not_balanced)
{
    // helpful definitions:
    TNode<T> *parent = not_balanced->getParent();
    TNode<T> *A = not_balanced->getRight();
    TNode<T> *temp = A->getLeft();
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
    A->updateHeight();
    A->updateBalance();
    return A;
}

template <class T>
TNode<T> *Tree<T>::LR(TNode<T> *not_balanced)
{
    TNode<T> *left_son = not_balanced->getLeft();
    RR(left_son);
    return LL(not_balanced);
}

template <class T>
TNode<T> *Tree<T>::RL(TNode<T> *not_balanced)
{
    TNode<T> *right_son = not_balanced->getRight();
    LL(right_son);
    return RR(not_balanced);
}

template <class T>
TNode<T> *Tree<T>::next_smaller(TNode<T> *vertice) const
{
    TNode<T> *parent = vertice->getParent();
    if (parent->getRight() == vertice)
    { // vertice is bigger than his parent
        return parent;
    }
    return next_smaller(parent);
}

template <class T>
TNode<T> *Tree<T>::next_bigger(TNode<T> *vertice) const
{
    TNode<T> *parent = vertice->getParent();
    if (parent->getLeft() == vertice)
    { // vertice is smaller than his parent
        return parent;
    }
    return next_bigger(parent);
}

template <class T>
TNode<T> *Tree<T>::rotate(TNode<T> *not_balanced)
{
    if (not_balanced == nullptr)
        return nullptr;
    not_balanced->updateHeight();
    not_balanced->updateBalance();
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
TNode<T> *Tree<T>::internalSearch(TNode<T> *node, int key_to_find) const
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
bool Tree<T>::isExist(int key_to_find) const
{
    return internalSearch(root, key_to_find) != nullptr;
}

template <class T>
T &Tree<T>::getData(int key_to_find) const
{
    return search(key_to_find).getData();
}

template <class T>
typename Tree<T>::const_iterator Tree<T>::search(const int key) const
{
    return Tree<T>::const_iterator(this, internalSearch(root, key));
}

template <class T>
TNode<T> *Tree<T>::internalInsert(TNode<T> *node, int key_to_insert, const T &data, TNode<T> *to_return)
{
    if (node == nullptr)
    {
        to_return = new TNode<T>(key_to_insert, data);
        size++;
        return to_return;
    }
    else
    {
        int key = node->getKey();
        if (key_to_insert == key)
        {
            //We chose to enable information updating//
            node->setData(data);
        }
        else if (key_to_insert > key)
        {
            TNode<T> *new_right = internalInsert(node->getRight(), key_to_insert, data, to_return);
            new_right->setParent(node);
            node->setRight(new_right);
        }
        else
        {
            TNode<T> *new_left = internalInsert(node->getLeft(), key_to_insert, data, to_return);
            new_left->setParent(node);
            node->setLeft(new_left);
        }
    }
    return rotate(node);
}

template <class T>
T &Tree<T>::insert(int key, const T &data)
{
    TNode<T> *to_return = nullptr;
    root = internalInsert(root, key, data, to_return);
    left_most = root->getMin();
    right_most = root->getMax();
    return to_return->getData();
}

template <class T>
TNode<T> *Tree<T>::internalRemove(TNode<T> *node, int key_to_remove)
{
    if (node == nullptr)
        return nullptr;
    int key = node->getKey();
    if (key_to_remove == key)
    {
        if (node->getRight() != nullptr && node->getLeft() != nullptr) // node has 2 sons
        {
            const TNode<T> *next_node = node->getRight()->getMin();
            node->setKey(next_node->getKey());
            node->setData(next_node->getDataConst());
            node->setRight(internalRemove(node->getRight(), next_node->getKey()));
        }
        else
        {
            TNode<T> *tmpNode = node;
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
        TNode<T> *new_right = internalRemove(node->getRight(), key_to_remove);
        if (new_right != nullptr)
        {
            new_right->setParent(node);
        }
        node->setRight(new_right);
    }
    else
    {
        TNode<T> *new_left = internalRemove(node->getLeft(), key_to_remove);
        if (new_left != nullptr)
        {
            new_left->setParent(node);
        }
        node->setLeft(new_left);
    }
    return rotate(node);
}

template <class T>
void Tree<T>::remove(int key)
{
    root = internalRemove(root, key);
    left_most = (root != nullptr) ? root->getMin() : nullptr;
    right_most = (root != nullptr) ? root->getMax() : nullptr;
}

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
TNode<T> *Tree<T>::internalArrayToTree(TNode<T> *parent, int *keys, T *array, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int middle = (start + end) / 2;
    TNode<T> *curr = new TNode<T>(keys[middle], array[middle], parent);
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
void Tree<T>::printTree(const std::string &prefix, const TNode<T> *node, bool isLeft) const
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

template <class T>
void Tree<T>::clearAll()
{
    cout << "before:" << endl;
    printTree();
    root = internalClear(root);
    cout << "after:" << endl;
    printTree();
}
#endif // TREE_H_