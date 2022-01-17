#ifndef LIST_H
#define LIST_H

#include <iostream>

using std::cout;
using std::endl;

////////CLASSES DECLERATION/////////
template <class T>
class Node
{
private:
    int key;
    T data;
    Node<T> *next;

public:
    Node();
    Node(int key, T &data);
    Node(const Node<T> &copy);
    ~Node() = default;
    T &getData();
    const int &getKey() const;
    void setData(T &to_add);
    Node<T> *getNext() const;
    void setNext(Node<T> *node);
};

template <class T>
class List
{
private:
    int size;
    Node<T> *head;
    Node<T> *tail;
    void DestroyList();
    Node<T> *internalSearch(int key) const;

public:
    class const_iterator;
    List();
    List(const List<T> &copy);
    ~List();
    List<T> &operator=(const List<T> &list);
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator search(int key) const;
    bool isExist(int key) const;
    void insert(int key, T data);
    void remove(const const_iterator &iterator);
    int length() const;
    void Print() const;

    // nested iterator
    class const_iterator
    {
    private:
        Node<T> *element;
        const_iterator(Node<T> *node);
        Node<T> *getNode() const;
        friend class List<T>;
        void setElement(Node<T> *node);

    public:
        const_iterator(const const_iterator &copy);
        ~const_iterator() = default;
        List<T>::const_iterator &operator=(const const_iterator &it);
        List<T>::const_iterator &operator++();
        bool operator==(const const_iterator &it) const;
        bool operator!=(const const_iterator &it) const;
        T &operator*();
        const int getKey() const;
    };
};

template <class T>
void List<T>::Print() const
{
    for (List<T>::const_iterator it = begin(); it != end(); ++it)
    {
        cout << "(" << it.getKey() << "," << *it  << ")-->";
    }
    cout << endl;
}

///////CLASSES IMPLEMENTATION/////////

//////// Node ////////
template <class T>
Node<T>::Node() : next(nullptr)
{
}

template <class T>
Node<T>::Node(int key, T &data) : key(key), data(data), next(nullptr)
{
}

template <class T>
Node<T>::Node(const Node<T> &copy)
{
    this->data = T(copy.data);
    this->next = nullptr;
}

template <class T>
T &Node<T>::getData() 
{
    return data;
}

template <class T>
const int &Node<T>::getKey() const
{
    return key;
}

template <class T>
void Node<T>::setData(T &to_add)
{
    data = to_add;
}

template <class T>
Node<T> *Node<T>::getNext() const
{
    return next;
}

template <class T>
void Node<T>::setNext(Node<T> *node)
{
    this->next = node;
}

//////// const_iterator ////////
template <class T>
List<T>::const_iterator ::const_iterator(Node<T> *node)
{
    this->element = node;
}

template <class T>
List<T>::const_iterator ::const_iterator(const const_iterator &copy)
{
    this->element = copy.element;
}

template <class T>
typename List<T>::const_iterator &List<T>::const_iterator ::operator=(const const_iterator &it)
{
    this->element = it.element;
    return *this;
}

template <class T>
typename List<T>::const_iterator &List<T>::const_iterator ::operator++()
{
    this->element = this->element->getNext();
    return *this;
}

template <class T>
bool List<T>::const_iterator ::operator==(const const_iterator &it) const
{
    if (this->element == it.element)
    {
        return true;
    }
    return false;
}

template <class T>
bool List<T>::const_iterator ::operator!=(const const_iterator &it) const
{
    return !(*this == it);
}

template <class T>
T &List<T>::const_iterator ::operator*() 
{
    return this->element->getData();
}

template <class T>
const int List<T>::const_iterator ::getKey() const
{
    return this->element->getKey();
}

template <class T>
void List<T>::const_iterator ::setElement(Node<T> *node)
{
    this->element = node;
}

template <class T>
Node<T> *List<T>::const_iterator ::getNode() const
{
    return this->element;
}

////////////////////////SORTED LIST////////////////////////////
template <class T>
List<T>::List() : size(0)
{
    this->head = new Node<T>();
    this->tail = head;
}

template <class T>
List<T>::List(const List<T> &copy) : size(0)
{
    this->head = new Node<T>();
    this->tail = this->head;
    for (const_iterator it = copy.begin(); it != copy.end(); ++it)
    {
        this->insert(*it);
    }
}

template <class T>
List<T>::~List()
{
    List<T>::DestroyList();
    delete head;
}

template <class T>
List<T> &List<T>::operator=(const List<T> &list)
{
    if (this == &list)
    {
        return *this;
    }

    this->DestroyList();
    for (const_iterator it = list.begin(); it != list.end(); ++it)
    {
        this->insert(*it);
    }
    return *this;
}

template <class T>
Node<T> *List<T>::internalSearch(int key) const
{
    Node<T> *current = head;
    while (current != nullptr)
    {
        if (current->getKey() == key)
            break;
        current = current->getNext();
    }
    return current;
}

template <class T>
bool List<T>::isExist(int key) const
{
    return (internalSearch(key) != nullptr);
}

template <class T>
typename List<T>::const_iterator List<T>::search(int key) const
{
    return const_iterator(internalSearch(key));
}

template <class T>
void List<T>::insert(int key, T data)
{
    Node<T> *new_node = new Node<T>(key, data);
    Node<T> *current = head;

    // checking if the key already exist
    while (current != nullptr)
    {
        if (current->getKey() == key)
            return;
        current = current->getNext();
    }

    // adding as first
    new_node->setNext(head);
    head = new_node;
    this->size++;
    return;
}

template <class T>
void List<T>::remove(const const_iterator &iterator)
{
    // no elements to remove
    if (size == 0 || end() == iterator)
    {
        return;
    }

    const_iterator it = begin();

    if (it == iterator)
    { // need to remove the first element
        head = it.getNode()->getNext();
        delete it.getNode();
        size--;
        return;
    }

    Node<T> *previous = it.getNode();
    ++it;

    for (; it != end(); ++it)
    {
        // need to remove it
        if (it == iterator)
        {
            previous->setNext(it.getNode()->getNext());
            delete it.getNode();
            this->size--;
            return;
        }

        previous = it.getNode();
    }
}

template <class T>
int List<T>::length() const
{
    return size;
}

template <class T>
typename List<T>::const_iterator List<T>::begin() const
{
    if (size == 0)
    {
        return end();
    }
    return const_iterator(head);
}

template <class T>
typename List<T>::const_iterator List<T>::end() const
{
    return const_iterator(tail);
}

template <class T>
void List<T>::DestroyList()
{
    Node<T> *current = head;

    while (nullptr != current)
    {
        Node<T> *next = current->getNext();
        delete current;
        current = next;
    }

    size = 0;
    head = new Node<T>();
    tail = head;
}
#endif // LIST_H
