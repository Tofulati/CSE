#ifndef ROBIN_SCHEDULE
#define ROBIN_SCHEDULE

#include "Queue.h"
#include <ostream>

class RobinSchedule {
    std::string names;
    int times;
    int value;
    int period = 0;
public:

    RobinSchedule(std::string name = "", int time = 0) {
        this->names = name;
        this->times = time;
    }

    void addPeriod(int value) {
        this->period = value;
        this->value = times/this->period + 1;
    }

    // int getPeriod() {
    //     return period;
    // }

    // int getTime() {
    //     return times;
    // }

    bool send() {
        value--;
        return value == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, RobinSchedule& r);
};


std::ostream& operator<<(std::ostream& os, RobinSchedule& r) {
    if (r.value > 0) {
        os << r.names << " - " << r.period;
    } else {
        os << r.names << " - " << r.times%r.period;
    }
    return os;
} 


#endif