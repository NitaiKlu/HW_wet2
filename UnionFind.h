#ifndef UF_H_
#define UF_H_

#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

#define DOES_NOT_EXIST -1

template <class T>
class UnionFind
{
private:
    struct
    {
        int parent;
        int size;
        T data;
    } typedef Node;
    Node *arr;
    int size;
    void checkBounds(int index) const; //group i is in arr[i-1]

public:
    UnionFind(int k);
    UnionFind(const UnionFind<T> &copy) = delete;
    ~UnionFind();

    void MakeSet(int i, T const &data);
    int Find(int i) const;
    T &getData(int i) const;
    void setData(T const &data);
    void Union(int p, int q);

    void printUnionFind() const;
};

template <class T>
UnionFind<T>::UnionFind(int k) : size(k)
{
    arr = new Node[k];
    for (int i = 0; i < k; i++)
    {
        arr[i].parent = DOES_NOT_EXIST;
        arr[i].size = DOES_NOT_EXIST;
    }
}

template <class T>
UnionFind<T>::~UnionFind()
{
    delete[] arr;
}

template <class T>
void UnionFind<T>::checkBounds(int index) const
{
    if (index <= 0 || index > size)
        throw std::out_of_range("Out of UF boundaries");
}

template <class T>
void UnionFind<T>::MakeSet(int i, T const &data)
{
    checkBounds(i);
    i--;
    arr[i].parent = i;
    arr[i].size = 1;
    arr[i].data = data;
}

// In the next 2 functions, we assume that 0<=i<=k
template <class T>
int UnionFind<T>::Find(int i) const
{
    checkBounds(i);
    i--;
    int root = i;
    // When the element is his own parent- he is the root of the up-tree
    while (root != arr[root].parent)
    {
        root = arr[root].parent;
    }
    // root is now the one we have looked for

    // Path compression
    while (i != root)
    {
        int tmp_parent = arr[i].parent;
        arr[i].parent = root;
        i = tmp_parent;
    }
    return root + 1;
}

template <class T>
T &UnionFind<T>::getData(int i) const
{
    checkBounds(i);
    return arr[Find(i)].data;
}

template <class T>
void UnionFind<T>::setData(T const &data)
{
    arr[i].data = data;
}

template <class T>
void UnionFind<T>::Union(int p, int q)
{
    int p_root = Find(p);
    int q_root = Find(q);
    if (p_root != q_root)
    {
        if (arr[p_root].size < arr[q_root].size)
        {
            arr[p_root].parent = q_root;
            arr[q_root].size += arr[p_root].size;
            arr[p_root].size = 0;
        }
        else
        {
            arr[q_root].parent = p_root;
            arr[p_root].size += arr[q_root].size;
            arr[q_root].size = 0;
        }
    }
}

template <class T>
void UnionFind<T>::printUnionFind() const
{
    cout << "size:   | ";
    for (int i = 0; i < size; i++)
    {
        if (arr[i].size != DOES_NOT_EXIST)
        {
            cout << arr[i].size << " | ";
        }
    }
    cout << endl;
    cout << "parent: | ";
    for (int i = 0; i < size; i++)
    {
        if (arr[i].size != DOES_NOT_EXIST)
        {
            cout << arr[i].parent << " | ";
        }
    }
    cout << endl;
    cout << "index:  | ";
    for (int i = 0; i < size; i++)
    {
        if (arr[i].size != DOES_NOT_EXIST)
        {
            cout << i << " | ";
        }
    }
    cout << endl
         << endl;
}
#endif // UF_H_