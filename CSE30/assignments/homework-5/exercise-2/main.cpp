#include <iostream>
#include <stdexcept>
#include <string>
#include "Queue.h"
#include "Stack.h"
#include "RobinSchedule.h"

using namespace std;


int main(){

    Queue<RobinSchedule> r;

    int jobCount;
    cin >> jobCount;

    for (int i = 0; i < jobCount; i++){
        string name;
        int time;
        cin >> name;
        cin >> time;
        r.enqueue(RobinSchedule(name, time));
    }

    int period;
    cin >> period;
    
    for (int i = 0; i < r.length(); i++) {
        RobinSchedule temp = r.dequeue();
        temp.addPeriod(period);
        r.enqueue(temp);
    }

    // Output round robin schedule, where person can use machine for no longer than the set period
    
    while (!r.isEmpty()) {
        RobinSchedule temp = r.dequeue();

        if (temp.send()) {
            cout << temp << endl;
        } else {
            cout << temp << endl;
            r.enqueue(temp);
        }
    }

    cout << "Done..." << endl;

    return 0;
}