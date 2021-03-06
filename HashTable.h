#ifndef HASH_H_
#define HASH_H_

#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

#define M 101
#define C 3
#define FACTOR 2
#define NOT_EXIST -1
#define ALREADY_THERE -2
#define DELETED -3
#define DECREASE_FACTOR 0.25

template <class T>
class Item
{
public:
    int key;
    T data;
    Item(int key) : key(key){};
    Item(int key, const T &data) : key(key), data(data){};
    ~Item() = default;
};

template <class T>
class HashTable
{
private:
    int size;        //size of the table
    int count;       //number of elements within the table
    Item<T> **items; //array of pointers to item - this is the table itself
    Item<T> *deleted;
    Item<T> *not_exist;
    int r(int x) const;
    int hash(int key, int index, int big) const; //double hashing
    void DestroyItems();
    int findMyHash(int key); //receives a key and returns the index of a free cell for it to be placed in it or ALREADY_THERE
    int findNextSize(int action);
    void reHash(int j); //enlarging the table
    void deHash();      //decreasing the table
    void printHashTable() const;

public:
    HashTable();
    HashTable(const HashTable<T> &copy) = delete;
    ~HashTable();
    int getCount() const;
    void insert(int key, const T &data);
    void remove(int key);
    bool isExist(int key) const;
    T &find(int key) const;
    int getSize() const;
    bool isExistAt(int index) const;
    T &getDataAt(int index) const;
};

/******************************
 * HASHTABLE FUNCTIONS
 * ****************************/
template <class T>
int HashTable<T>::r(int x) const
{ //ensuring that gcd(r(x),2) = 1 && gcd(r(x), M) = 1
    x = x % (M - C);
    if (x % 2 != 0) 
    {
        x--;
    }
    return 1 + x; //x != 0 for sure
}

template <class T>
int HashTable<T>::hash(int key, int index, int big) const
{ //double hashing
    int hash1 = key % big;
    int hash2 = r(key); //!= 0
    return (hash1 + hash2 * index) % big;
}

template <class T>
HashTable<T>::HashTable() : size(M), count(0)
{
    items = new Item<T> *[M];
    deleted = new Item<T>(DELETED);
    not_exist = new Item<T>(NOT_EXIST);
    for (int i = 0; i < M; i++)
    {
        items[i] = not_exist;
    }
}

template <class T>
int HashTable<T>::findNextSize(int action)
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
void HashTable<T>::DestroyItems()
{
    for (int i = 0; i < size; i++)
    {
        if (items[i] != deleted && items[i] != not_exist)
        {
            delete items[i];
        }
    }
}

template <class T>
HashTable<T>::~HashTable()
{
    DestroyItems();
    delete[] items;
    delete deleted;
    delete not_exist;
}

template <class T>
int HashTable<T>::getCount() const
{
    return count;
}

template <class T>
int HashTable<T>::findMyHash(int key)
{
    int j = 0;
    int index = hash(key, j, size);
    while (j < size && items[index]->key > 0) //looking for a free spot
    {                                         //this cell is not DELETED and not NOT_EXIST
        index = hash(key, j, size);
        if (items[index]->key == key) //the element we passed through has the same key
        {
            return ALREADY_THERE;
        }
        j++;
    }
    if (j == size) //couldn't find a spot
    {
        return NOT_EXIST;
    }
    return index;
}

template <class T>
void HashTable<T>::deHash()
{
    //creating and inizializing a new_items array
    Item<T> **new_items;
    int new_size = findNextSize(-1);
    new_items = new Item<T> *[new_size];
    for (int i = 0; i < new_size; i++)
    {
        new_items[i] = not_exist;
    }
    int previous = size;
    //transferring the elements
    for (int i = 0; i < previous; i++)
    {
        int key, index, j = 0;
        key = items[i]->key;
        if (key > 0) //items[i] exists.
        {
            //there is an element to copy from this cell
            index = hash(key, j, new_size);
            while (j < new_size && new_items[index] != not_exist) //looking for a free spot
            {
                index = hash(key, j, new_size);
                j++;
            }
            /*****************not suppose to get here:
             * what happens if j can't find a spot?
             * deleting new_items
             * ****************/
            if (j == new_size)
            {
                delete[] new_items;
                return;
            }
            new_items[index] = items[i];
        }
    }
    delete[] items;
    size = new_size;
    items = new_items;
}

template <class T>
void HashTable<T>::reHash(int j)
{
    //creating and inizializing a new_items array
    Item<T> **new_items;
    int new_size = findNextSize(j);
    new_items = new Item<T> *[new_size];
    for (int i = 0; i < new_size; i++)
    {
        new_items[i] = not_exist;
    }
    int previous = size;
    //transferring the elements
    for (int i = 0; i < previous; i++)
    {
        int key, index, new_i = 0;
        key = items[i]->key;
        if (key > 0) //items[i] exists.
        {
            //there is an element to copy from this cell
            index = hash(key, new_i, new_size);
            while (new_i < new_size && new_items[index] != not_exist) //looking for a free spot
            {
                index = hash(key, new_i, new_size);
                new_i++;
            }
            /***************** not suppose to get here but:
             * what happens if j can't find a spot?
             * deleting new_items
             * reHashing with double the size of new_items
             * ****************/
            if (new_i == new_size && new_items[index] != not_exist)
            {
                delete[] new_items;
                reHash(j + 1);
                return;
            }
            new_items[index] = items[i];
        }
    }
    delete[] items;
    size = new_size;
    items = new_items;
}

template <class T>
void HashTable<T>::insert(int key, const T &data)
{
    if (count == size - 1) //the table is full
    {
        reHash(1);
    }
    int index = findMyHash(key); //looking for a free cell
    if (index == NOT_EXIST)      //couldn't find a spot (not suppose to happen)
    {
        reHash(1); //enlarge the table
        index = findMyHash(key);
    }
    else if (index == ALREADY_THERE) //no adding twice the same key in my table
    {
        return;
    }
    //found a spot! adding to the table
    items[index] = new Item<T>(key, data);
    count++;
}

template <class T>
void HashTable<T>::remove(int key)
{
    Item<T> *temp;
    int i;
    if (count == 0 || !isExist(key)) //no data to delete
    {                                //do nothing if key wasn't found
        return;
    }
    for (i = 0; i < size; i++)
    {
        int index = hash(key, i, size);
        if (items[index]->key == key) //key was found
        {
            temp = items[index];
            delete temp;
            items[index] = deleted;
            count--;
            break;
        }
    }
    if (count <= DECREASE_FACTOR * size && size > M) //need to decrease table size
    {
        deHash();
    }
}

template <class T>
bool HashTable<T>::isExist(int key) const
{
    int i;
    if (count == 0) //no data to find
    {
        return false;
    }
    for (i = 0; i < size; i++)
    {

        if (items[hash(key, i, size)]->key == key)
        {
            return true;
        }
        if (items[hash(key, i, size)]->key == NOT_EXIST)
        {
            return false;
        }
    }
    return false;
}

template <class T>
T &HashTable<T>::find(int key) const //assumes that the item is inside (using previous func)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (items[hash(key, i, size)]->key == key)
        {
            return items[hash(key, i, size)]->data;
        }
    }
    return items[hash(key, i, size)]->data; //trash if nothing was found
}

template <class T>
void HashTable<T>::printHashTable() const
{
    for (int i = 0; i < size; i++)
    {
        if (items[i] != deleted && items[i]->key != NOT_EXIST)
        {
            cout << "element number " << i << endl;
            cout << "key: " << items[i]->key << ", data: " << items[i]->data << endl;
        }
    }
}

template <class T>
int HashTable<T>::getSize() const
{
    return size;
}

//assumes index is in array-size boundries
template <class T>
T &HashTable<T>::getDataAt(int index) const
{
    return items[index]->data;
}

template <class T>
bool HashTable<T>::isExistAt(int index) const
{
    if (index >= size || index < 0)
        return false;
    if (items[index] == deleted || items[index] == not_exist)
        return false;
    return true;
}

#endif //HASH_H_
