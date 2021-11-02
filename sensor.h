#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <string>
using std::string;

#include "timelineStorage.h"

class Sensor{
    public :
        Sensor(string identifier, TimeLineStorage* timeLineStorage);

        virtual void measure(string measurementIdentifier) = 0;
    protected:
        TimeLineStorage* _timeLineStorage;
        std::map<string, string> tags;
};

#endif