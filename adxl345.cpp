#include <iostream>
#include <string>
using std::string;
#include <cmath>

#include "adxl345.h"

// Power consumption Settings
#define POWER_MODES_KEY     0x2C
#define POWER_MODE_VALUE    0x0F

#define STANDBY_MODE_KEY    0x2D
#define STANDBY_MODE_VALUE  0x08

// General Settings
#define DATA_FORMAT_KEY     0x31
#define DATA_FORMAT_VALUE   0x0B


ADXL345::ADXL345(string identifier, TimeLineStorage* timeLineStorage, int spiChannel) : SPISensor(identifier, timeLineStorage, spiChannel) {
    initialiseTheSensor();

    accelerationConversionFactor = 2 * 16.0 / 8192.0;
}
void ADXL345::initialiseTheSensor(){
    char data[] = {POWER_MODE_VALUE, POWER_MODES_KEY};
    sendBytes(data, 2);

    data[0] = STANDBY_MODE_KEY;
    data[1]  = STANDBY_MODE_VALUE;
    sendBytes(data, 2);

    // Resolution and range settings could be modified with this register
    data[0] = DATA_FORMAT_KEY;
    data[1]  = DATA_FORMAT_VALUE;
    sendBytes(data, 2);
} 

void ADXL345::measure(string measurementIdentifier){

    unsigned int steps = 100;

    char data[7];
    int bytes;
    
    for (unsigned int i = 0; i < steps; ++i)
    {
        data[0] = 0x32;
        bytes = transceiveBytes(data, 7);
        int16_t x, y, z;
        x = (data[2]<<8)|data[1];
        y = (data[4]<<8)|data[3];
        z = (data[6]<<8)|data[5];

        std::map<string, string> fields;

        fields["a_x"] = std::to_string(x * accelerationConversionFactor);
        fields["a_y"] = std::to_string(y * accelerationConversionFactor);
        fields["a_z"] = std::to_string(z * accelerationConversionFactor);

        _timeLineStorage->store(measurementIdentifier, tags, fields);
    }
    
}