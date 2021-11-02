#ifndef ADXL345asdfasdf
#define ADXL345asdfasdf

#include <map>
using std::string;

#include "SPISensor.h"

class ADXL345 : SPISensor{
    public:
        ADXL345(string identifier, TimeLineStorage* timeLineStorage, int spiChannel);

        void measure(string measurementIdentifier) override;
    
    private:
        void initialiseTheSensor();

        double accelerationConversionFactor;
};


#endif