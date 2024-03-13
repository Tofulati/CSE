#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"
template <class T>
class Stack;

template <class T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& q);


template <class T>
class Stack {
    // Your code here
    LinkedList<T> elements;
public:
    void push(T value) {
        elements.prepend(value);
    }

    T pop() {
        return elements.removeFirst();
    }

    T peek() {
        return elements.peek();
    }

    bool isEmpty() const {
        return elements.isEmpty();
    }

    int length() const {
        return elements.size();
    }
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& q){
    // Your code here
    os << q.elements;
    return os;
}

#endif