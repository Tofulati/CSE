#ifndef ARRAY_LIST
#define ARRAY_LIST

// Your array list implementation here...
#include <ostream>
#include <iostream>
#include <stdexcept>

template <class T>
class ArrayList;

template <class T> 
std::ostream& operator<<(std::ostream& os, const ArrayList<T>& value);

template <class T>
class ArrayList {
public:
    int count;
    int capacity;
    T* arr;

    void inflate() {
        if (count == capacity) {
            int oldCapacity = capacity;
            capacity *= 2;

            T* temp = new T[capacity];
            for (int i = 0; i < oldCapacity; i++) {
                temp[i] = arr[i];
            }

            T* old = arr;
            arr = temp;
            
            delete[] old;
        }
    }

    void deflate() {
        if (count * 2 < capacity) {
            capacity /= 2;

            T* temp = new T[capacity];
            for (int i = 0; i < count; i++) {
                temp[i] = arr[i];
            }

            T* old = arr;
            arr = temp;
            
            delete[] old;
        }
    }

    ArrayList() {
        count = 0;
        capacity = 1;
        arr = new T[capacity];
    }

    ArrayList(const ArrayList<T>& other) {
        capacity = other.capacity;
        count = other.count;
        arr = new T[capacity];
        for (int i = 0; i < count; i++) {
            arr[i] = other.arr[i];
        }
    }

    ArrayList& operator=(const ArrayList<T>& other) {
        if (capacity == other.capacity) {
            count = other.capacity;
            for (int i = 0; i < count; i++) {
                arr[i] = other.arr[i];
            }
        } else {
            T* old = arr;
            capacity = other.capacity;
            count = other.count;
            arr = new T[capacity];

            for (int i = 0; i < count; i++) {
                arr[i] = other.arr[i];
            }

            delete[] old;
        }

        return *this;
    }

    void append(T value) {
        arr[count] = value;
        count++;
        inflate();
    }

    void prepend(T value) {
        for (int i = count; i > 0; i--) {
            arr[i] = arr[i - 1]; 
        }
        arr[0] = value;
        count++;
        inflate();
    }

    T& operator[](int index) {
        if (index < 0 || index >= count) {
            std::logic_error("Can not remove from invalid index");
        }
        return arr[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= count) {
            std::logic_error("Can not remove from invalid index");
        }
        return arr[index];
    }

    T remove(int index) {
        if (index < 0 || index >= count) {
            std::logic_error("Can not remove from invalid index");
        }
        if (count == 0) {
            std::logic_error("Can not remove from empty list");
        }

        T result = arr[index];

        for (int i = index; i < count - 1; i++) {
            arr[i] = arr[i + 1];
        }
        
        count--;
        deflate();

        return result;
    }

    T removeFirst() {
        if (count == 0) {
            std::logic_error("Can not remove from empty list");
        }

        T result = arr[0];

        for (int i = 0; i < count - 1; i++) {
            arr[i] = arr[i + 1];
        }

        count--;
        deflate();

        return result;
    }

    T removeLast() {
        if (count == 0) {
            std::logic_error("Can not remove from empty list");
        }

        T result = arr[count - 1];

        count--;
        deflate();

        return result;
    }

    bool search(T value) const {
        for (int i = 0; i < count; i++) {
            if (arr[i] == value) {
                return true;
            }
        }
        return false;
    }

    int size() {
        return count;
    } 

    ~ArrayList() {
        delete[] arr;
    }

    friend std::ostream& operator<<(std::ostream& os, const ArrayList<T>& list) {
        for (int i = 0; i < list.count; i++) {
            os << list.arr[i];
            if (i < list.count - 1) {
                os << ",";
            }
        }

        return os;
    }
};

#endif