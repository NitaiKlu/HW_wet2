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
    struct Info
    {
        int group_size;
        T data;
        Info(T const &data) : group_size(1), data(data) {};
    }; 
    struct Node
    {
        int parent;
        Info *item;
    };
    Node *arr;
    int num_of_groups;
    void checkBounds(int index) const; //group i is in arr[i-1]

public:
    UnionFind(int num_of_groups);
    UnionFind(const UnionFind<T> &copy) = delete;
    ~UnionFind();

    void MakeSet(int p, T const &data);
    int Find(int p) const;
    T &getData(int p) const;
    void setData(int p, T const &data);
    void Union(int p, int q);

    void printUnionFind() const;
};

template <class T>
UnionFind<T>::UnionFind(int num_of_groups) : num_of_groups(num_of_groups)
{
    arr = new Node[num_of_groups];
    for (int i = 0; i < num_of_groups; i++)
    {
        arr[i].parent = DOES_NOT_EXIST;
        arr[i].item = nullptr;
    }
}

template <class T>
UnionFind<T>::~UnionFind()
{
    for (int i = 0; i < num_of_groups; i++)
    {
        if (arr[i].item != nullptr)
        {
            delete arr[i].item;
        }
    }
    delete[] arr;
}

template <class T>
void UnionFind<T>::checkBounds(int index) const
{
    if (index <= 0 || index > num_of_groups)
        throw std::out_of_range("Out of UF boundaries");
}

template <class T>
void UnionFind<T>::MakeSet(int i, T const &data)
{
    checkBounds(i);
    i--;
    if (arr[i].parent != DOES_NOT_EXIST)
    {
        return;
    }
    arr[i].parent = i;
    arr[i].item = new Info(data);
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
    return arr[Find(i) - 1].item->data;
}

template <class T>
void UnionFind<T>::setData(int p, T const &data)
{
    arr[Find(p) - 1].item->data = data;
}

template <class T>
void UnionFind<T>::Union(int p, int q)
{
    int p_root = Find(p)-1;
    int q_root = Find(q)-1;
    if (p_root != q_root)
    {
        if (arr[p_root].item->group_size < arr[q_root].item->group_size)
        {
            arr[p_root].parent = q_root;
            arr[q_root].item->group_size += arr[p_root].item->group_size;
            delete arr[p_root].item;
            arr[p_root].item = nullptr;
        }
        else
        {
            arr[q_root].parent = p_root;
            arr[p_root].item->group_size += arr[q_root].item->group_size;
            delete arr[q_root].item;
            arr[q_root].item = nullptr;
        }
    }
}

/*
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
*/
#endif // UF_H_