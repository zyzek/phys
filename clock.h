#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

using namespace std::chrono;

class Clock
{

public:
    Clock():
        instantiated(high_resolution_clock::now()), lastQuery(instantiated) {}

    double elapsed();
    double delta();

private:
    high_resolution_clock::time_point instantiated, lastQuery;

};

#endif // CLOCK_H
