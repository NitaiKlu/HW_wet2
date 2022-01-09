#ifndef RNODE_H_
#define RNODE_H_

template <class T>
class RNode
{
private:
    int key;
    int height;
    int balance;
    RNode<T> *parent;
    RNode<T> *left;
    RNode<T> *right;
    T data;
    // sizes and weights have a size of num_of_weights+1
    // sizes[0] = 1
    // weights[0] = "normal weight"
    int num_of_weights;
    int *sizes;
    int *weights;
    void internalUpdateWeight(int index);
    void checkBounds(int index);

public:
    RNode();
    RNode(int num_of_weights, int key, const T &data, RNode<T> *parent = nullptr);
    RNode(const RNode<T> &copy) = delete;
    ~RNode();
    T &getData();
    const T &getDataConst() const;
    void setData(const T &new_data);
    const int getKey() const;
    const int getHeight() const;
    const int getBalance() const;
    int getNumOfWeights() const;
    bool isLeaf() const;
    RNode<T> *getParent() const;
    RNode<T> *getLeft() const;
    RNode<T> *getRight() const;
    RNode<T> *getMin();
    RNode<T> *getMax();
    int getSizeAt(int i) const;
    int getWeightAt(int i) const;
    int getWeight() const;
    void setParent(RNode<T> *node);
    void setLeft(RNode<T> *node);
    void setRight(RNode<T> *node);
    void setKey(int key); // dangerous function! only used by Tree
    void updateBalance();
    void updateHeight();
    void addToSize(int index, int addition);
    void updateAllWeights();
    void updateWeightAt(int index);
};

template <class T>
RNode<T>::RNode(int num_of_weights, int key, const T &data, RNode<T> *parent)
    : num_of_weights(num_of_weights), key(key), height(0), balance(0), parent(parent), left(nullptr), right(nullptr), data(data)
{
    // mem check?????
    sizes = new int[num_of_weights + 1];
    weights = new int[num_of_weights + 1];
    sizes[0] = 1;
    weights[0] = 1;
    for (int i = 1; i <= num_of_weights; i++)
    {
        sizes[i] = 0;
        weights[i] = 0;
    }
}

template <class T>
RNode<T>::~RNode()
{
    delete[] weights;
    delete[] sizes;
}

template <class T>
void RNode<T>::updateBalance()
{
    int l_height = (left == nullptr) ? -1 : left->height;
    int r_height = (right == nullptr) ? -1 : right->height;
    balance = l_height - r_height;
}

template <class T>
void RNode<T>::updateHeight()
{
    int l_height = (left == nullptr) ? -1 : left->height;
    int r_height = (right == nullptr) ? -1 : right->height;
    height = std::max(l_height, r_height) + 1;
}
template <class T>
void RNode<T>::checkBounds(int index)
{
    if (index <= 0 || index > num_of_weights)
        throw std::out_of_range("Out of rank boundaries");
}

template <class T>
void RNode<T>::internalUpdateWeight(int index)
{
    // updating the weight at index
    int l_weight = (left == nullptr) ? 0 : left->weights[index];
    int r_weight = (right == nullptr) ? 0 : right->weights[index];
    weights[index] = l_weight + r_weight + sizes[index];
    /*
    // updating the size sum (at[num_of_weights])
    int l_weight_sum = (left == nullptr) ? 0 : left->weights[num_of_weights];
    int r_weight_sum = (right == nullptr) ? 0 : right->weights[num_of_weights];
    weights[index] = l_weight + r_weight + sizes[num_of_weights];
    */
}

template <class T>
void RNode<T>::addToSize(int index, int addition)
{
    //assuming index is checked at tree level
    sizes[index] += addition;
    //sizes[num_of_weights] += addition;
    //internalUpdateWeight(index);
    weights[index] += addition;
}

/*
template <class T>
void RNode<T>::updateSize(int index, int size)
{

    int old_size;
    if (index >= 0 && index < num_of_weights)
    {
        old_size = sizes[index];
        sizes[index] = size;
    }
    sizes[num_of_weights] += size - old_size;
    updateWeight(index);

    int addition = size - sizes[index];
    addToSize(index, addition);
}
*/

template <class T>
void RNode<T>::updateWeightAt(int index)
{
    //assuming index is checked at tree level
    internalUpdateWeight(index);
}

template <class T>
void RNode<T>::updateAllWeights()
{
    for (int i = 0; i <= num_of_weights; i++)
    {
        internalUpdateWeight(i);
    }
}

template <class T>
int RNode<T>::getSizeAt(int i) const
{
    //assuming index is checked at tree level
    return sizes[i];
}

template <class T>
int RNode<T>::getWeightAt(int i) const
{
    //assuming index is checked at tree level
    return weights[i];
}

template <class T>
int RNode<T>::getWeight() const
{
    return getWeightAt(0);
}

template <class T>
T &RNode<T>::getData()
{
    return data;
}

template <class T>
const T &RNode<T>::getDataConst() const
{
    return data;
}

template <class T>
const int RNode<T>::getKey() const
{
    return key;
}

template <class T>
void RNode<T>::setKey(int key)
{
    this->key = key;
}

template <class T>
const int RNode<T>::getHeight() const
{
    return height;
}

template <class T>
const int RNode<T>::getBalance() const
{
    return balance;
}

template <class T>
int RNode<T>::getNumOfWeights() const
{
    return num_of_weights;
}

template <class T>
void RNode<T>::setData(const T &new_data)
{
    data = new_data;
}

template <class T>
bool RNode<T>::isLeaf() const
{
    return left == nullptr && right == nullptr;
}

template <class T>
RNode<T> *RNode<T>::getParent() const
{
    return parent;
}

template <class T>
RNode<T> *RNode<T>::getLeft() const
{
    return left;
}

template <class T>
RNode<T> *RNode<T>::getRight() const
{
    return right;
}

template <class T>
RNode<T> *RNode<T>::getMin()
{
    if (left == nullptr)
    {
        return this;
    }
    return left->getMin();
}

template <class T>
RNode<T> *RNode<T>::getMax()
{
    if (right == nullptr)
    {
        return this;
    }
    return right->getMax();
}

template <class T>
void RNode<T>::setParent(RNode<T> *node)
{
    if(node == nullptr)
        parent = nullptr;
    else
        parent = node;
}

template <class T>
void RNode<T>::setLeft(RNode<T> *node)
{
    if(node == nullptr)
        left = nullptr;
    else
        left = node;
}

template <class T>
void RNode<T>::setRight(RNode<T> *node)
{
    if(node == nullptr)
        right = nullptr;
    else
        right = node;
}

#endif // RNODE_H_
