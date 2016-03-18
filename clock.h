#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

using namespace std::chrono;

class Clock
{

public:
    Clock():
        instantiated(high_resolution_clock::now()), last_query(instantiated) {}

    virtual ~Clock() {}

    double elapsed();
    double delta();

private:
    high_resolution_clock::time_point instantiated, last_query;

};

#endif // CLOCK_H
