#ifndef NODE_H
#define NODE_H
#include <string>
#include <sstream>
using namespace std;

template <typename T>
class Node {
private:

public:
    T value;
    Node<T>* next;
    Node<T>* prev;
    Node(T _value);
    ~Node();
    Node(T _value, Node<T>* _next);
    Node(T _value, Node<T>* _prev, Node<T>* _next);
    T getElement();

    /**Getters & Setters**/
    Node<T>* getNext();
    Node<T>* getPrev();
    Node<T>* setNext(const Node<T>* next);
    Node<T>* setPrev(const Node<T>* prev);
    /********************/


    string to_string(T const& value);
    const string toString();
};

template <typename T>
Node<T>::Node(T _value) {
    value = _value;
    next = __null;
    prev = __null;
}

template <typename T>
Node<T>::~Node() {
}

template <typename T>
Node<T>::Node(T _value, Node<T>* _next) {
    value = _value;
    next = _next;
}

template <typename T>
Node<T>::Node(T _value, Node<T>* _prev, Node<T>* _next) {
    value = _value;
    this->prev =  _prev;
    this->next = _next;
}

template <typename T>
T Node<T>::getElement() {
    return value;
}

template <typename T>
Node<T>* Node<T>::getNext() {
    return next;
}

template <typename T>
Node<T>* Node<T>::getPrev() {
    return prev;
}

template <typename T>
Node<T>* Node<T>::setNext(const Node<T>* next) {
    this->next = next;
}

template <typename T>
Node<T>* Node<T>::setPrev(const Node<T>* prev) {
    this->prev = prev;
}

template <typename T>
string Node<T>::to_string(T const& value) {
    stringstream sstr;
    sstr << value;
    return sstr.str();
}

template <typename T>
const string Node<T>::toString() {
    stringstream ss;
    ss << value;
    string str = ss.str();
    return "(" + str + ")";
}


#endif /* NODE_H */

