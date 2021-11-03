#ifndef ADXL345asdfasdf
#define ADXL345asdfasdf

#include <map>
#include <chrono>
using std::string;

#include "SPISensor.h"

class ADXL345 : public SPISensor{
    public:
        ADXL345(string identifier, TimeLineStorage* timeLineStorage, unsigned int maxSamplingFrequenzy, int spiChannel);

    protected:
        void specificMeasure(string measurementIdentifier, std::chrono::_V2::system_clock::time_point now) override;

    private:
        void initialiseTheSensor();

        double accelerationConversionFactor;
};


#endif