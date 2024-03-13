#ifndef CONTAINER_H
#define CONTAINER_H

#include "ArrayList.h"
#include "LinkedList.h"

struct Container{

    ArrayList<int> storage;

    Container(){
        for (int i = 0; i <= 5000; i++) {
            storage.append(0);
        }
    }

    Container(int value) {
        for (int i = 0; i <= value; i++) {
            storage.append(0);
        }
    }

    void add(int value){
        storage[value]++;
    }


    bool search(int value){
        return storage[value] > 0; 
    }


};

#endif