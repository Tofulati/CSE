#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "ArrayList.h"
#include "LinkedList.h"

struct HashTable{

    // You can declare more instance variables here
    int K;

    ArrayList<LinkedList<int>> ht;

    HashTable(){
        K = 10;
        // If you have more instance variables, init them here
        for (int i = 0; i <= K; i++) {
            ht.append(LinkedList<int>());
        }
    }

    HashTable(int K){
        this->K = K;
        // If you have more instance variables, init them here
        for (int i = 0; i <= K; i++) {
            ht.append(LinkedList<int>());
        }
    }

    void add(int value){
        // This function must be implemented
        int pos = f(value);

        ht[pos].append(value);
    }


    bool search(int value){
        // This function must be implemented
        int pos = f(value);

        if (ht[pos].search(value)) {
            return true;
        } else {
            return false;
        }
    }

    // You can declare other functions if needed
    int f(int x) {
        int result = x % K;

        // not sure if you could do: int result = abs(x % K);

        if (result < 0) {
            result *= -1;
        }

        return result;
    }

};

#endif