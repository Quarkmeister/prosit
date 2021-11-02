#ifndef ADXL345asdfasdf
#define ADXL345asdfasdf

#include <map>
using std::string;

#include "SPISensor.h"

class ADXL345 : SPISensor{
    public:
        ADXL345(string identifier, TimeLineStorage* timeLineStorage, unsigned int maxSamplingFrequenzy, int spiChannel);

        void measure(std::__cxx11::string measurementIdentifier) override;
    
    private:
        void initialiseTheSensor();

        double accelerationConversionFactor;
};


#endif