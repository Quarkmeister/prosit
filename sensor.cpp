#include "sensor.h"

Sensor::Sensor(string identifier, TimeLineStorage* timeLineStorage){
    _timeLineStorage = timeLineStorage;

    tags["sensor"] = identifier;
}
