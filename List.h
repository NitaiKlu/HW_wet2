#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <iostream>

using std::cout;
using std::endl;

namespace mtm
{
  ////////CLASSES DECLERATION/////////
  template <class T>
  class Node {
      private:
        T data;
        Node<T>* next;

      public:
        Node();
        Node(T& data);
        Node(const Node<T>& copy);
        ~Node() = default;
        const T& getData() const;
        void setData(T& to_add);
        Node<T>* getNext() const;
        void setNext(Node<T>* node);
  };

  template <class T>
  class SortedList {
      private:
          int size;
          Node<T>* sorted_list;
          Node<T>* last;
          void DestroyList();
      public:
          class const_iterator;
          SortedList();
          SortedList(const SortedList<T>& copy);
          ~SortedList();
          SortedList<T>& operator=(const SortedList<T>& list);
          const_iterator begin() const;
          const_iterator end() const;
          void insert(T t);
          void remove(const const_iterator& iterator);
          int length() const;
          template <class Predicate>
          SortedList<T> filter(Predicate p) const;
          template <class Appliance>
          SortedList<T> apply(Appliance a) const;
          void Print() const;

      //nested iterator
      class const_iterator {
      private:
          Node<T>* element;
          const_iterator(Node<T>* node);
          Node<T>* getNode() const;
          friend class SortedList<T>;
          void setElement(Node<T>* node);
      public:
          const_iterator(const const_iterator &copy);
          ~const_iterator() = default;
          SortedList<T> :: const_iterator& operator=(const const_iterator& it);
          SortedList<T> :: const_iterator& operator++();
          bool operator==(const const_iterator& it) const;
          bool operator!=(const const_iterator& it) const;
          const T& operator*() const;
      }; 
  };

  template <class T>
  void SortedList<T> :: Print() const {
      for(SortedList<T>::const_iterator it = begin(); it != end(); ++it) {
          cout << *it << endl; 
      }
      cout << endl; 
  }

  ///////CLASSES IMPLEMENTATION/////////

  //////// Node ////////
  template <class T>
  Node<T> :: Node() 
  {
      this->next = NULL;
  } 

  template <class T>
  Node<T> :: Node(T& t)
  {
      data = T(t);
      next = NULL;
  }

  template <class T>
  Node<T> :: Node(const Node<T>& copy)
  {
      this->data = T(copy.data);
      this->next = NULL;
  }

  template <class T>
  const T& Node<T> :: getData() const 
  {
      return data;
  }

  template <class T>
  void Node<T> :: setData(T& to_add) 
  {
      data = to_add; 
  }

  template <class T>
  Node<T>* Node<T> :: getNext() const 
  {
      return next;
  }

  template <class T>
  void Node<T> :: setNext(Node<T>* node) {
      this->next = node;
  }

  //////// const_iterator ////////
  template <class T>
  SortedList<T> :: const_iterator :: const_iterator(Node<T>* node) {
      this->element = node;
  }


  template <class T>
  SortedList<T> :: const_iterator :: const_iterator(const const_iterator &copy) {
      this->element = copy.element;
  }

  template <class T>
  typename SortedList<T> :: const_iterator& SortedList<T> :: const_iterator :: operator=(const const_iterator &it) 
  {
      this->element = it.element;
      return *this;
  }

  template <class T>
  typename SortedList<T> :: const_iterator& SortedList<T> :: const_iterator :: operator++() {
      this->element = this->element->getNext();
      return *this;
  }

  template <class T>
  bool SortedList<T> :: const_iterator :: operator==(const const_iterator &it) const {
      if(this->element == it.element)
      {
          return true;
      }
      return false;
  }

  template <class T>
  bool SortedList<T> :: const_iterator :: operator!=(const const_iterator &it) const {
      return !(*this == it);
  }

  template <class T>
  const T& SortedList<T> :: const_iterator :: operator*() const{
      return this->element->getData();
  }

  template <class T>
  void  SortedList<T> :: const_iterator :: setElement(Node<T>* node) {
      this->element = node; 
  }

  template <class T>
  Node<T>* SortedList<T> :: const_iterator :: getNode() const {
      return this->element;
  }


  ////////////////////////SORTED LIST////////////////////////////
  template <class T>
  SortedList<T> :: SortedList() : size(0)
  {
      this->sorted_list = new Node<T>();
      this->last = sorted_list;
  } 

  template <class T>
  SortedList<T> :: SortedList(const SortedList<T> &copy) : size(0) 
  {
      this->sorted_list = new Node<T>();
      this->last = this->sorted_list;
      for(const_iterator it = copy.begin(); it != copy.end(); ++it) {
          this->insert(*it);
      }
  }

  template <class T>
  SortedList<T> :: ~SortedList() 
  {
      SortedList<T> :: DestroyList();
      delete sorted_list;
  }

  template <class T>
  SortedList<T>& SortedList<T> :: operator=(const SortedList<T>& list)
  {
      if(this == &list) {
          return *this;
      }

      this->DestroyList();
      for(const_iterator it = list.begin(); it != list.end(); ++it) {
          this->insert(*it);
      }
      return *this;
  }

  template <class T>
  void SortedList<T> :: insert(T to_add) {
      Node<T>* new_node = new Node<T>(to_add);
      Node<T>* current = sorted_list;

      //checking if needs to be added as first
      if (size == 0 || new_node->getData() < current->getData()) { 
          new_node->setNext(current);  
          sorted_list = new_node;
          this->size++;
          return;    
      }

      Node<T>* previous = current;
      current = current->getNext();
      //searching for new_node's location in the list
      while (current != this->last && current->getData() < new_node->getData()) {
          previous = current;
          current = current->getNext();
      }
      
      //new node is added wherever the loop stopped
      previous->setNext(new_node);
      new_node->setNext(current);
      this->size++;
  }

  template <class T>
  void SortedList<T> :: remove(const const_iterator& iterator)
  {
    // no elements to remove
      if (size == 0 || end() == iterator) {
          return;
      }

      const_iterator it = begin();
      
      if (it == iterator) { //need to remove the first element
        sorted_list = it.getNode()->getNext();
        delete it.getNode(); 
        size--;
        return;
      }

      Node<T>* previous = it.getNode();
      ++it;

      for ( ;it != end(); ++it) {
        //need to remove it
        if(it == iterator) {
          previous->setNext(it.getNode()->getNext());
          delete it.getNode(); 
          this->size--;
          return;
        }
        
        previous = it.getNode();
      }
  }

  template <class T>
  int SortedList<T> :: length() const 
  {
      return size;
  }

  template<class T>
  template<class Predicate>
  SortedList<T> SortedList<T> :: filter(Predicate p) const
  {
      const_iterator it = begin();
      SortedList<T> filtered_list = SortedList<T>();
      for (const_iterator it = begin(); it != end(); ++it) {
        if (p(*it)) {
          filtered_list.insert(*it);    
        }
      }
      return filtered_list;
  }

  template <class T>
  template <class Appliance>
  SortedList<T> SortedList<T> :: apply(Appliance a) const
  {
      SortedList<T> applied_list = SortedList<T>();
      for (const_iterator it = begin(); it != end(); ++it) {
        applied_list.insert(a(*it));
      }
      return applied_list;
  }


  template<class T>
  typename SortedList<T>::const_iterator SortedList<T> :: begin() const
  {
      if (size == 0) {
          return end();
      }
      return const_iterator(sorted_list);
  }

  template<class T>
  typename SortedList<T>::const_iterator SortedList<T> :: end() const
  {
      return const_iterator(this->last);
  }

  template<class T>
  void SortedList<T> :: DestroyList() 
  {
      Node<T> *current = sorted_list;

      while (NULL != current) {
        Node<T> *next = current->getNext();
        delete current;
        current = next;
      }

      size = 0;
      sorted_list = new Node<T>();
      last = sorted_list;
  }
} // namespace mtm
#endif //SORTEDLIST_H


