#include "clock.h"

using namespace std::chrono;

double Clock::elapsed()
{
    high_resolution_clock::time_point t = high_resolution_clock::now();
    duration<double> passed = t - instantiated;
    return passed.count();
}

double Clock::delta()
{
    high_resolution_clock::time_point t = high_resolution_clock::now();
    duration<double> passed = t - last_query;
    last_query = t;
    return passed.count();
}

