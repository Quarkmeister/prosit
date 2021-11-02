#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <string>
using std::string;
#include <chrono>

#include "timelineStorage.h"

class Sensor{
    public :
        Sensor(string identifier, TimeLineStorage* timeLineStorage, unsigned int maxSamplingFrequenzy);

        virtual void measure(string measurementIdentifier);

    protected:
        TimeLineStorage* _timeLineStorage;
        std::map<string, string> tags;

    private:
        double maxSamplingPeriod; //[ns]
        std::chrono::_V2::system_clock::time_point timeOfLastMeasure;
        std::chrono::_V2::system_clock::time_point now();
};

#endif