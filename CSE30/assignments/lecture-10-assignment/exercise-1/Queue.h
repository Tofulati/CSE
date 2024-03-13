#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"
template <class T>
class Queue;

template <class T>
std::ostream& operator<<(std::ostream& os, const Queue<T>& q);


template <class T>
class Queue{
    // Your code here
    LinkedList<T> elements;
public:
    void enqueue(T value) {
        elements.append(value);
    }

    T dequeue() {
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
std::ostream& operator<<(std::ostream& os, const Queue<T>& q){
    // Your code here
    os << q.elements;
    return os;
}

#endif