#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include "ArrayList.h"

template <typename T>

class SortedList : public ArrayList<T> {
public:
    // struct Link {
    //     T data;
    //     Link* next;

    //     Link() {
    //         data = 0;
    //         next = nullptr;
    //     }

    //     Link(T value) {
    //         data = value;
    //         next = nullptr;
    //     }
    // };

    // Link* front;
    // Link* back;

    // void append(T value){
    //     // Your code here
    //     if (front == nullptr) {
    //         front = new Link(value);
    //         back = front;
    //     } else {
    //         Link* temp = new Link(value);
    //         back->next = temp;
    //         back = temp;
    //     }
    // }

    // bool search(T value) const {
    //     // Your code here
    //     Link* curr = front;

    //     while (curr != nullptr) {
    //         if (curr->data == value) {
    //             return true;
    //         }
    //         curr = curr->next;
    //     } 
    //     return false;
    // }

    void append(T value) {
        int left = 0;
        int right = this->count;
        int insert_index = 0;
        
        while (left <= right) {
            insert_index = (left + right) / 2;

            if (this->arr[insert_index] == value) { 
                break;
            } else {
                if (this->arr[insert_index] > value) {
                    right = insert_index - 1;
                } else {
                    left = insert_index + 1;
                }
            }
        }

        for (int i = this->count; i > insert_index; i--) {
            this->arr[i] = this->arr[i-1];
        }
        this->arr[insert_index] = value;

        this->count++;
        this->inflate();
    }

    bool search(T value) const {
        int left = 0;
        int right = this->count - 1;

        while (left <= right){
            int mid = (left + right) / 2;

            if (this->arr[mid] == value){
                return true;
            }
            else{
                if (this->arr[mid] > value){
                    right = mid - 1;
                }
                else {
                    left = mid + 1;
                }
            }
        }
        return false;
    }

    void insertionSort() { //too slow
        for (int i = 1; i < this->count + 1; i++) {
            T key = this->arr[i];
            int j = i - 1;

            while (j >= 0 && this->arr[j] > key) {
                this->arr[j + 1] = this->arr[j];
                j = j -1;
            }
            this->arr[j + 1] = key;
        }
    }  

    // This needs to be here because of the unit tests
    friend struct TestSortedList;

};

#endif