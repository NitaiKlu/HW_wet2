#ifndef CHASH_H_
#define CHASH_H_

#include <iostream>
#include <memory>
#include "List.h"
#define M 7
#define FACTOR 2
#define NOT_EXIST -1
#define ALREADY_THERE -2
#define DECREASE_FACTOR 0.25


template <class T>
class ChainHash
{
private:
    int size;                         //size of the table
    int count;                        //number of elements within the table
    List<T> **items;                  //array of pointers to lists<T>
    int hash(int key, int big) const; //double hashing
    void DestroyItems();
    int findMyHash(int key); //receives a key and returns the index of a free cell for it to be placed in it or ALREADY_THERE
    int findNextSize(int action);
    void reHash(); //enlarging the table
    void deHash(); //decreasing the table

public:
    ChainHash();
    ChainHash(const ChainHash<T> &copy) = delete;
    ~ChainHash();
    int getCount() const;
    void insert(int key, const T &data);
    void remove(int key);
    bool isExist(int key) const;
    T &find(int key) const;
    int getSize() const;
    /**bool isExistAt(int index) const;
    T &getDataAt(int index) const;**/
    ChainHash<T>& operator+=(const ChainHash<T>& hash_table);
    void printHashTable() const;
};

/******************************
 * ChainHash FUNCTIONS
 * ****************************/

template <class T>
int ChainHash<T>::hash(int key, int big) const
{
    int hash1 = key % big;
    /**int r = 1 + key % (big - 3);
    return (hash1 + (r + 1) * index) % big;**/
    return hash1;
}

template <class T>
ChainHash<T>::ChainHash() : size(M), count(0)
{
    items = new List<T> *[M];
    for (int i = 0; i < M; i++)
    {
        items[i] = new List<T>();
    }
}

template <class T>
int ChainHash<T>::findNextSize(int action)
{
    if (action > 0)
    { //enlarge
        return size * FACTOR * action;
    }
    if (action == -1)
    { //decrease
        return size * DECREASE_FACTOR * 2;
    }
    return size;
}

template <class T>
void ChainHash<T>::DestroyItems()
{
    for (int i = 0; i < size; i++)
    {
        delete items[i];
    }
}

template <class T>
ChainHash<T>::~ChainHash()
{
    DestroyItems();
    delete[] items;
}

template <class T>
int ChainHash<T>::getCount() const
{
    return count;
}

template <class T>
void ChainHash<T>::deHash()
{
    //creating and inizializing a new_items array
    List<T> **new_items;
    int new_size = findNextSize(-1);
    new_items = new List<T> *[new_size];
    for (int i = 0; i < new_size; i++)
    {
        new_items[i] = new List<T>();
    }
    int previous = size;
    int new_spot, key;
    //transferring the elements
    for (int i = 0; i < previous; i++)
    {
        for (auto it = items[i]->begin(); it != items[i]->end(); ++it)
        {
            key = it.getKey();
            new_spot = hash(key, new_size);
            new_items[new_spot]->insert(key, *it);
        }
    }
    delete[] items;
    size = new_size;
    items = new_items;
}

template <class T>
void ChainHash<T>::reHash()
{
    //creating and inizializing a new_items array
    List<T> **new_items;
    int new_size = findNextSize(1);
    new_items = new List<T> *[new_size];
    for (int i = 0; i < new_size; i++)
    {
        new_items[i] = new List<T>();
    }
    int previous = size;
    int new_spot, key;
    //transferring the elements
    for (int i = 0; i < previous; i++)
    {
        for (auto it = items[i]->begin(); it != items[i]->end(); ++it)
        {
            key = it.getKey();
            new_spot = hash(key, new_size);
            new_items[new_spot]->insert(key, *it);
        }
    }
    delete[] items;
    size = new_size;
    items = new_items;
}

template <class T>
void ChainHash<T>::insert(int key, const T &data)
{
    if (isExist(key))
    //no adding twice the same key in my table
    {
        return;
    }
    int spot = hash(key, size);
    items[spot]->insert(key, data);
    count++;
    if(count/size > FACTOR) { //checking load factor
        reHash();
    }
}

template <class T>
void ChainHash<T>::remove(int key)
{
    if (count == 0 || !isExist(key)) //no data to delete
    {                                //do nothing if key wasn't found
        return;
    }
    int index = hash(key, size);
    items[index]->remove(items[index]->search(key));
    if (count <= DECREASE_FACTOR * size && size > M) //need to decrease table size
    {
        deHash();
    }
    count--;
}

template <class T>
bool ChainHash<T>::isExist(int key) const
{
    if (count == 0) //no data to find
    {
        return false;
    }
    int index = hash(key, size);
    return items[index]->isExist(key);
}

template <class T>
T &ChainHash<T>::find(int key) const //assumes that the item is inside (using previous func)
{
    int index = hash(key, size);
    return *(items[index]->search(key));
}

template <class T>
int ChainHash<T>::getSize() const
{
    return size;
}

template <class T>
ChainHash<T>& ChainHash<T>::operator+=(const ChainHash<T>& hash_table) 
{   
    int table_size = hash_table.getSize();
    for(int i = 0; i < table_size; i++)
    {
        List<T>* curr = hash_table.items[i];
        for(auto it = curr->begin(); it != curr->end(); ++it)
        {
            insert(it.getKey(), *it);
        }
    }
    return *this;
}

template <class T>
void ChainHash<T>::printHashTable() const
{
    for (int i = 0; i < size; i++)
    {
        std::cout << "#" << i << ": ";
        items[i]->Print();
    }
}


#endif //HASH_H_
