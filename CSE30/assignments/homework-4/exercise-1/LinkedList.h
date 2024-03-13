#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <ostream>
#include <stdexcept>
template <class T>

struct Link{
    T data;
    Link<T>* next;

    Link<T>(){
        data = T();
        next = nullptr;
    }

    Link<T>(T data){
        this->data = data;
        next = nullptr;
    }
};

template <class T>
class LinkedList{
    Link<T>* front;
    Link<T>* back;
    int size;
public:
    LinkedList<T>(){
        front = nullptr;
        back = nullptr;
        size = 0;
    }

    void append(T value){
        if (front == nullptr){
            // Appending to an empty list
            front = new Link<T>(value);
            back = front;
        }
        else{
            // Appending to list with elements
            Link<T>* temp = new Link<T>(value);
            back->next = temp;
            back = temp;
        }
        size++;
    }

    T& operator[](int index){
        if (index < 0 || index >= size){
            throw std::logic_error("Index out of bounds");
        }
        Link<T>* temp = front;
        for (int i = 0; i < index; i++){
            temp = temp->next;
        }
        return temp->data;
    }

    void prepend(T value){
        if (front == nullptr){
            // Prepending to an empty list
            front = new Link<T>(value);
            back = front;
        }
        else{
            // Prepending to a list with elements
            Link<T>* temp = new Link<T>(value);
            temp->next = front;
            front = temp;
        }
        size++;
    }

    T removeFirst() {
        if (size == 0) {
            throw std::logic_error("Can not remove first");
        }

        T result = front->data;

        Link<T>* old = front;

        front = front->next;

        if (front == nullptr) {
            back = nullptr;
        }

        size--;

        delete old; 

        return result;
    }

    T removeLast() {
        if (size == 0) {
            throw std::logic_error("Can not remove last");
        }

        if (front->next == nullptr) {
            T result = front->data;
            delete front;
            front = nullptr;
            back = nullptr;

            size--;

            return result;
        }

        T result = back->data;

        Link<T>* prev = nullptr;
        Link<T>* curr = front;
        while (curr != nullptr) {
            prev = curr;
            curr = curr->next;
        }

        delete curr;
        back = prev;
        back->next = nullptr;

        size--;

        return result;
    }


    friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list){
        Link<T>* curr = list.front;
        while (curr != nullptr){
            os << curr->data;
            if (curr->next != nullptr) os << ", ";
            curr = curr->next;
        }

        return os;
    }
    
    // The following 2 friend declarations must be present so the unit tests can run
    friend struct TestTemplates;
    friend struct TestRemoveMethods;

};

#endif