#ifndef NODE_H_
#define NODE_H_

template <class T>
class TNode
{
private:
    int key;
    int height;
    int balance;
    TNode<T> *parent;
    TNode<T> *left;
    TNode<T> *right;
    T data;

public:
    TNode();
    explicit TNode(int key, const T &data, TNode<T> *parent = nullptr);
    TNode(const TNode<T> &copy) = delete;
    ~TNode() = default;
    T &getData();
    const T &getDataConst() const;
    void setData(const T &new_data);
    const int getKey() const;
    const int getHeight() const;
    const int getBalance() const;
    bool isLeaf() const;
    TNode<T> *getParent() const;
    TNode<T> *getLeft() const;
    TNode<T> *getRight() const;
    TNode<T> *getMin();
    TNode<T> *getMax();
    void setParent(TNode<T> *node);
    void setLeft(TNode<T> *node);
    void setRight(TNode<T> *node);
    void setKey(int key); //dangerous function! only used by Tree
    void updateBalance();
    void updateHeight();
    int calculate_update_balance();
};

template <class T>
TNode<T>::TNode(int key, const T &data, TNode<T> *parent)
    : key(key), height(0), balance(0), parent(parent), left(nullptr), right(nullptr), data(data)
{
}

template <class T>
void TNode<T>::updateBalance()
{
    int l_height = (left == nullptr) ? -1 : left->height;
    int r_height = (right == nullptr) ? -1 : right->height;
    balance = l_height - r_height;
}

template <class T>
void TNode<T>::updateHeight()
{
    int l_height = (left == nullptr) ? -1 : left->height;
    int r_height = (right == nullptr) ? -1 : right->height;
    height = std::max(l_height, r_height) + 1;
}

template <class T>
T &TNode<T>::getData()
{
    return data;
}

template <class T>
const T &TNode<T>::getDataConst() const
{
    return data;
}

template <class T>
const int TNode<T>::getKey() const
{
    return key;
}

template <class T>
void TNode<T>::setKey(int key)
{
    this->key = key;
}

template <class T>
const int TNode<T>::getHeight() const
{
    return height;
}

template <class T>
const int TNode<T>::getBalance() const
{
    return balance;
}

template <class T>
void TNode<T>::setData(const T &new_data)
{
    data = new_data;
}

template <class T>
bool TNode<T>::isLeaf() const
{
    return left == nullptr && right == nullptr;
}

template <class T>
TNode<T> *TNode<T>::getParent() const
{
    return parent;
}

template <class T>
TNode<T> *TNode<T>::getLeft() const
{
    return left;
}

template <class T>
TNode<T> *TNode<T>::getRight() const
{
    return right;
}

template <class T>
TNode<T> *TNode<T>::getMin()
{
    if (left == nullptr)
    {
        return this;
    }
    return left->getMin();
}

template <class T>
TNode<T> *TNode<T>::getMax()
{
    if (right == nullptr)
    {
        return this;
    }
    return right->getMax();
}

template <class T>
void TNode<T>::setParent(TNode<T> *node)
{
    parent = node;
}

template <class T>
void TNode<T>::setLeft(TNode<T> *node)
{
    left = node;
}

template <class T>
void TNode<T>::setRight(TNode<T> *node)
{
    right = node;
}

template <class T>
int TNode<T>::calculate_update_balance()
{
    balance = left->height - right->height;
    return balance;
}

#endif // NODE_H_
