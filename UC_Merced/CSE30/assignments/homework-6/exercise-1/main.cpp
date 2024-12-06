#include <iostream>
#include <utility>
#include "LinkedList.h"
#include "TimeSupport.h"
#include "RandomSupport.h"
#include "ArrayList.h"
#include "Container.h"

using namespace std;



int main(){

    Container bag;
    bag.add(5);
    bag.add(7);
    bag.add(3);
    bag.add(1);
    bag.add(4);

    cout << bag.search(0) << endl;
    cout << bag.search(1) << endl;
    cout << bag.search(2) << endl;
    cout << bag.search(3) << endl;
    cout << bag.search(4) << endl;
    cout << bag.search(5) << endl;
    cout << bag.search(6) << endl;
    cout << bag.search(7) << endl;
    cout << bag.search(8) << endl;
    cout << bag.search(9) << endl;


    // const int N = 10000;

    // Container arr(N);
    // RNG rng(0, N-1);


    // timestamp start = current_time();

    // for (int i = 0; i < N; i++){
    //     int num = rng.get();
    //     arr.add(i);

    //     for (int j = 0; j < N; j++){
    //        arr.search(-1);
    //     }
    // }

    // timestamp end = current_time();

    // int duration = time_diff(start, end);

    // cout << "Took " << duration << " ms." << endl;

    // return 0;
}