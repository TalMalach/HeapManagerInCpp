#ifndef LIST_H
#define LIST_H


#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class List {
public:

    List() : head(__null), tail(__null), list_size(0) {
    }
    List(const size_t size);
    List(const List& other);
    ~List();

    List<T>& operator=(const List& other);

    /**Getters**/
    Node<T>* getHead();
    Node<T>* getTail();


    T ElementAt(const size_t position);

 /**Operator Overloading**/
    T operator[](const size_t position);


    void push_back(const T value);
    void pop_back();
    void remove(const size_t position);
    const string toString();


    size_t size() const {
        return list_size;
    }


private:

    void exception();

    Node<T> *head;
    Node<T> *tail;
    size_t list_size;
};

template <typename T>
List<T>::List(const size_t size): head(__null), tail(__null), list_size(size){}

template <typename T>
List<T>::List(const List& other) {
    List::operator=(other);
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
    list_size = other.list_size;
    Node<T> i;
    for (Node<T>* j = other.head, *n = &i; j; j = j->next) {
        n->next = new Node<T>(j);
        n = n->next;
    }
    this->head = i.next;
    return *this;
}

template <typename T>
List<T>::~List() {
    Node<T>*NodePtr = head;
    while (NodePtr != tail) {
        Node<T>* NodeNext =NodePtr->next;
        delete NodePtr;
       NodePtr = NodeNext;
    }
    delete tail;
}

template <typename T>
T List<T>::ElementAt(const size_t position) {
    Node<T>*NodePtr = head;
    if (position < 0 || position >= this->size()) {
        exception();
    } else {

        for (size_t i = 0; i < position; i++) {
           NodePtr =NodePtr->next;
        }
    }
    return NodePtr->value;
}

template <typename T>
Node<T>* List<T>::getHead() {
    return head;
}

template <typename T>
Node<T>* List<T>::getTail() {
    return tail;
}

template <typename T>
T List<T>::operator[](const size_t position) {
    return ElementAt(position);
}

template <typename T>
void List<T>::push_back(const T value) {

    Node<T>* newNode = new Node<T>(value, tail, head);
    if (head == __null) {
        head = tail = newNode;

    }
    else {
        tail->next = newNode;
        head->prev = newNode;
        tail = newNode;
    }

    ++list_size;
}

template <typename T>
void List<T>::pop_back() {
    if (!head) {
        return;
    } else if (list_size == 1) {
        delete head;
        head = tail = __null;
        list_size = 0;
    } else {

        Node<T>*NodePtr = tail;
        Node<T>*Node_Prev = tail->prev;
       Node_Prev->next = head;
        head->prev =Node_Prev;

        delete NodePtr;
        tail =Node_Prev;
        --list_size;
    }
}

template <typename T>
const string List<T>::toString() {
    if (list_size == 0) return "empty";
    std::string str = "->";
    Node<T> *current = head;
    while (current != tail) {
        str += current->toString() + "->";
        current = current->next;
    }
    str += current->toString() + "->";
    return str;
}

template <typename T>
void List<T>::remove(const size_t position) {
    if (list_size <= position || head == __null) {
        exception();
    }
    Node<T>*NodePtr = head;
    Node<T>*Node_Prev = __null;
    if (position == 0) {
        tail->next = head->next;
        head->next->prev = tail;
        head = head->next;

        delete NodePtr;
    }
    if (position == list_size - 1) {
       NodePtr = tail;
        tail->prev->next = head->next;
        head->prev = tail->prev;
        tail = tail->prev;

        delete NodePtr;
    } else {
        for (size_t i = 0; i < position; i++) {
           Node_Prev =NodePtr;
           NodePtr =NodePtr->next;
        }
       Node_Prev->next =NodePtr->next;
       NodePtr->next->prev =Node_Prev;
        delete NodePtr;
    }
    --list_size;
}

template <typename T>
void List<T>::exception() {
    std::cout << "out of range."<<std::endl;
}
#endif /* LIST_H */

