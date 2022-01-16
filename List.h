#ifndef LIST_H
#define LIST_H

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
  class List {
      private:
          int size;
          Node<T>* ls;
          Node<T>* last;
          void DestroyList();
      public:
          class const_iterator;
          List();
          List(const List<T>& copy);
          ~List();
          List<T>& operator=(const List<T>& list);
          const_iterator begin() const;
          const_iterator end() const;
          void insert(T t);
          void remove(const const_iterator& iterator);
          int length() const;
          void Print() const;

      //nested iterator
      class const_iterator {
      private:
          Node<T>* element;
          const_iterator(Node<T>* node);
          Node<T>* getNode() const;
          friend class List<T>;
          void setElement(Node<T>* node);
      public:
          const_iterator(const const_iterator &copy);
          ~const_iterator() = default;
          List<T> :: const_iterator& operator=(const const_iterator& it);
          List<T> :: const_iterator& operator++();
          bool operator==(const const_iterator& it) const;
          bool operator!=(const const_iterator& it) const;
          const T& operator*() const;
      }; 
  };

  template <class T>
  void List<T> :: Print() const {
      for(List<T>::const_iterator it = begin(); it != end(); ++it) {
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
  List<T> :: const_iterator :: const_iterator(Node<T>* node) {
      this->element = node;
  }


  template <class T>
  List<T> :: const_iterator :: const_iterator(const const_iterator &copy) {
      this->element = copy.element;
  }

  template <class T>
  typename List<T> :: const_iterator& List<T> :: const_iterator :: operator=(const const_iterator &it) 
  {
      this->element = it.element;
      return *this;
  }

  template <class T>
  typename List<T> :: const_iterator& List<T> :: const_iterator :: operator++() {
      this->element = this->element->getNext();
      return *this;
  }

  template <class T>
  bool List<T> :: const_iterator :: operator==(const const_iterator &it) const {
      if(this->element == it.element)
      {
          return true;
      }
      return false;
  }

  template <class T>
  bool List<T> :: const_iterator :: operator!=(const const_iterator &it) const {
      return !(*this == it);
  }

  template <class T>
  const T& List<T> :: const_iterator :: operator*() const{
      return this->element->getData();
  }

  template <class T>
  void  List<T> :: const_iterator :: setElement(Node<T>* node) {
      this->element = node; 
  }

  template <class T>
  Node<T>* List<T> :: const_iterator :: getNode() const {
      return this->element;
  }


  ////////////////////////SORTED LIST////////////////////////////
  template <class T>
  List<T> :: List() : size(0)
  {
      this->ls = new Node<T>();
      this->last = ls;
  } 

  template <class T>
  List<T> :: List(const List<T> &copy) : size(0) 
  {
      this->ls = new Node<T>();
      this->last = this->ls;
      for(const_iterator it = copy.begin(); it != copy.end(); ++it) {
          this->insert(*it);
      }
  }

  template <class T>
  List<T> :: ~List() 
  {
      List<T> :: DestroyList();
      delete ls;
  }

  template <class T>
  List<T>& List<T> :: operator=(const List<T>& list)
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
  void List<T> :: insert(T to_add) {
      Node<T>* new_node = new Node<T>(to_add);
      Node<T>* current = ls;

      //checking if needs to be added as first
      if (size == 0 || new_node->getData() < current->getData()) { 
          new_node->setNext(current);  
          ls = new_node;
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
  void List<T> :: remove(const const_iterator& iterator)
  {
    // no elements to remove
      if (size == 0 || end() == iterator) {
          return;
      }

      const_iterator it = begin();
      
      if (it == iterator) { //need to remove the first element
        ls = it.getNode()->getNext();
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
  int List<T> :: length() const 
  {
      return size;
  }

  template<class T>
  typename List<T>::const_iterator List<T> :: begin() const
  {
      if (size == 0) {
          return end();
      }
      return const_iterator(ls);
  }

  template<class T>
  typename List<T>::const_iterator List<T> :: end() const
  {
      return const_iterator(this->last);
  }

  template<class T>
  void List<T> :: DestroyList() 
  {
      Node<T> *current = ls;

      while (NULL != current) {
        Node<T> *next = current->getNext();
        delete current;
        current = next;
      }

      size = 0;
      ls = new Node<T>();
      last = ls;
  }
}
#endif //LIST_H


