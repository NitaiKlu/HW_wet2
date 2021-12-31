#ifndef HASH_H_
#define HASH_H_

#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

#define M 41
#define PRIME 37
#define FACTOR 2
#define NOT_EXIST -1

template <class T>
class HashTable
{
private:
    int size; //size of the table
    int count; //number of elements within the table
    struct
    {
        int key;
        shared_ptr<T> data;
    } typedef Item;

    Item** items; //array of pointers to item - this is the table itself
    int hash(int key, int index); //double hashing
public:
    HashTable();
    HashTable(const HashTable<T> &copy);
    ~HashTable();
    void reHash();
    void insert(int key, T data);
    void remove(int key);
    bool isExist(int key);
    T& find(int key);
    void printHashTable();
};


/******************************
 * HASHTABLE FUNCTIONS
 * ****************************/

template <class T>
int HashTable<T>::hash(int key, int index)
{
    int hash1 = key % size;
    int hash2 = PRIME - (key % PRIME);
    return (hash1 + index*hash2) % size;
}

template <class T>
HashTable<T>::HashTable() : size(M), count(0) 
{
    items = new Item*[M];
    for(int i=0; i<M; i++)
    {
        items[i] = new Item;
        items[i]->key = NOT_EXIST;
        items[i]->data = nullptr;
    }
}

template <class T>
HashTable<T>::HashTable(const HashTable<T> &copy) : size(copy.size), count(copy.count)
{
    items =new shared_ptr<Item>[size];
    for(int i=0; i<size; i++)
    {
        items[i] = copy.items[i];
    }
}

template <class T>
HashTable<T>::~HashTable()
{
    delete items;
}

template <class T>
void HashTable<T>::reHash()
{
    Item** new_items;
    *new_items = new Item[size*FACTOR];
    int previous = size;
    size = size*FACTOR;
    int key;
    for(int i=0; i < size; i++)
    {
        int j=0;
        if(items[i]->key != NOT_EXIST) { //there is an element to copy from this cell
            key = items[i]->key; 
            while(j < size && new_items[hash(key, j)]->key == NOT_EXIST) //looking for a free spot
            {
                j++;
            }
            if(j == size - 1) { //still couldn't find a spot
                size = previous;
                reHash();
            }
            new_items[hash(key, j)]->key = items[i]->key;
            new_items[hash(key, j)]->data = items[i]->data;
        }  
    }
    delete items;
    items = new_items;
}

template <class T>
void HashTable<T>::insert(int key, T data)
{
    if(count == size-1) //the table is full
    {
        reHash(); 
    }
    int i = 0;
    while(items[hash(key, i)]->key != NOT_EXIST && i < size) //keep hashing until we find free spot
    {
        i++;
    }
    if(i == size - 1) //still couldn't find a spot
    {
        reHash();
        insert(key, data);
    }
    else 
    {
        int spot = hash(key, i); 
        items[spot]->key =key;
        items[spot]->data = make_shared<T>(data);
    }
    count++;
}

template <class T>
void HashTable<T>::remove(int key)
{
    int i;
    if(count == 0) //no data to delete
    {
        return;
    }
    for (i = 0; i < size; i++)
    {
        if(items[hash(key, i)]->key == key) {
            break;
        }
    }
    if(i != size) //key was found
    {
        items[hash(key,i)]->key = NOT_EXIST;
        items[hash(key,i)]->data = nullptr;
        count--;
    }
    
}

template<class T>
bool HashTable<T>::isExist(int key)
{
    int i;
    if(count == 0) //no data to find
    {
        return false;
    }
    for (i = 0; i < size; i++)
    {
        if(items[hash(key, i)]->key == key) {
            return true;
        }
    }
    return false;
}

template<class T>
T& HashTable<T>::find(int key)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if(items[hash(key, i)]->key == key) {
            return *(items[hash(key, i)]->data);
        }
    }
    return items[hash(key, i)]->key;
}

template<class T>
void HashTable<T>::printHashTable()
{
    for (int i = 0; i < size; i++)
    {
        if(items[i]->data != nullptr) {
            cout << "element number " << i << endl;
            cout << "key: "<< items[i]->key <<", data: " << *(items[i]->data) << endl;
        }
    }
}

#endif //HASH_H_

