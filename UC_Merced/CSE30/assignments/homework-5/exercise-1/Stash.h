#ifndef STASH_H
#define STASH_H

#include "Stack.h"
#include <stdexcept>
#include <string>

// Your code here
class Stash {
    Stack<int> elements;
public:
    void deposit(int value) {
        elements.push(value);
    }

    void withdraw(int value) {
        elements.push(-1*value);
    }
    
    int getBalance() {
        int total = 0;
        Stack<int> temp;
        while (!elements.isEmpty()) {
            int value = elements.pop();
            temp.push(value);
            total += value;
        }

        while (!temp.isEmpty()) {
            elements.push(temp.pop());
        }

        return total;
    }

    void undo() {
        if (elements.isEmpty()) {
            throw std::logic_error("No operations to undo");
        }
        elements.pop();
    }
};

#endif