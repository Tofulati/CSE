#include <iostream>
#include "ArrayList.h"

using namespace std;

int main(int argc, char* argv[]){

    // You can experiment with your ArrayList data structure here.

    ArrayList<int> numbers;
    numbers.append(99);
    numbers.append(27);
    numbers.append(14);
    numbers.append(42);
    numbers.append(72);

    numbers.removeFirst();

    numbers.stats();

    return 0;
}