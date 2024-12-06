#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Your linked list implementation here

#include <ostream>
#include <iostream>
#include <stdexcept>

template <class T>
struct Link;

template <class T>
class LinkedList;

template <class T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& value);

template <class T>
struct Link {
    Link* next;
    Link* prev;
    T data;

    Link() {
        next = nullptr;
        prev = nullptr;
        data = 0;
    }

    Link(T value) {
        next = nullptr;
        prev = nullptr;
        data = value;
    }
};

template <class T>
class LinkedList {
public:
    Link<T>* front;
    Link<T>* back;
    int count;

    LinkedList() {
        front = nullptr;
        back = nullptr;
        count = 0;
    }

    LinkedList(LinkedList<T>& other) {
        front = nullptr;
        back = nullptr;
        count = 0;
        
        Link<T>* curr = other.front;
        while (curr != nullptr) {
            append(curr->data);
            curr = curr->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        while (front != nullptr) {
            Link<T>* temp = front;
            front = front->next;
            delete temp;
        }
        back = nullptr;
        count = 0;
        Link<T>* curr = other.front;
        while (curr != nullptr) {
            append(curr->data);
            curr = curr->next;
        }

        return *this;
    }

    void append(T value) {
        if (front == nullptr) {
            front = new Link<T>(value);
            back = front;
        } else {
            Link<T>* node = new Link<T>(value);
            back->next = node;
            node->prev = back;
            back = node;
        }

        count++;
    }

    void prepend(T value) {
        if (front == nullptr) {
            front = new Link<T>(value);
            back = front;
        } else {
            Link<T>* node = new Link<T>(value);
            node->next = front;
            front->prev = node;
            front = node;
        }

        count++;
    }

    bool isEmpty() {
        return count == 0;
    }

    int size() {
        return count;
    }

    T removeFirst() {
        if (front == nullptr) {
            throw std::logic_error("Can not remove from empty list");
        }
        if (count == 1) {
            T result = front->data;
            delete front;
            front = nullptr;
            back = nullptr;
            count--;

            return result;
        } else {
            T result = front->data;
            Link<T>* old = front;
            front = front->next;
            front->prev = nullptr;
            delete old;

            count--;

            return result;
        }
    }

    T removeLast() {
        if (front == nullptr) {
            throw std::logic_error("Can not remove from empty list");
        }
        if (count == 1) {
            T result = front->data;
            delete front;
            front = nullptr;
            back = nullptr;
            count--;

            return result;
        } else {
            T result = back->data;
            Link<T>* old = back;
            back = back->prev;
            back->next = nullptr;
            delete old;

            count--;

            return result;
        }
    }

    T getFirst() {
        if (front == nullptr) {
            throw std::logic_error("Can not get from empty list");
        }
        return front->data;
    }

    T getLast() {
        if (front == nullptr) {
            throw std::logic_error("Can not get from empty list");
        }
        return back->data;
    }

    void reverse() {
        Link<T>* curr = front;
        while (curr != nullptr) {
            Link<T>* temp = curr->next;
            curr->next = curr->prev;
            curr->prev = temp;
            curr = temp;
        }

        Link<T>* old = front;
        front = back;
        back = old;
    }

    bool search(T value) {
        if (front == nullptr) {
            throw std::logic_error("Can not get from empty list");
        }

        Link<T>* curr = front;
        while (curr != nullptr) {
            if (curr->data == value) {
                return true;
            }
            curr = curr->next;
        }

        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& value) {
        Link<T>* curr = value.front;
        while (curr != nullptr) {
            os << curr->data;
            if (curr->next != nullptr) {
                os << ",";
            }
            curr = curr->next;
        }
        
        return os;
    }
};

#endif