#include "sensor.h"

#include <iostream>
using std::chrono::time_point_cast;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::nanoseconds;

Sensor::Sensor(string identifier, TimeLineStorage* timeLineStorage, unsigned int maxSamplingFrequenzy){
    _timeLineStorage = timeLineStorage;

    maxSamplingPeriod = 1.0 / maxSamplingFrequenzy * 1'000'000'000;
    timeOfLastMeasure = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());

    tags["sensor"] = identifier;
}

void Sensor::measure(string measurementIdentifier){

    // This routine ensafes that the maximum read frequency of the sensor isnt exceed
    auto timePointNow = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
    
    if(duration_cast<nanoseconds>(timePointNow-timeOfLastMeasure).count() > maxSamplingPeriod){

        timeOfLastMeasure = time_point_cast<nanoseconds>(std::chrono::high_resolution_clock::now());

        this->specificMeasure(measurementIdentifier, timePointNow);
    }
}
