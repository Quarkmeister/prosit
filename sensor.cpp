#include "sensor.h"

Sensor::Sensor(string identifier, TimeLineStorage* timeLineStorage, unsigned int maxSamplingFrequenzy){
    _timeLineStorage = timeLineStorage;

    maxSamplingPeriod = 1.0 / maxSamplingFrequenzy * 1'000'000'000;
    timeOfLastMeasure = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());

    tags["sensor"] = identifier;
}

void Sensor::measure(string measurementIdentifier){
    
    while(std::chrono::duration_cast<std::chrono::nanoseconds>(now()-timeOfLastMeasure).count() < maxSamplingPeriod){}

    timeOfLastMeasure = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());

}

 std::chrono::_V2::system_clock::time_point Sensor::now(){
     return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());
 } 
