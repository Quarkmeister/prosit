#ifndef SPI_SENSOR_h
#define SPI_SENSOR_h

#include "sensor.h"

class SPISensor : protected Sensor
{
    public:
        SPISensor(string identifier, TimeLineStorage* timeLineStorage, int spiChannel);
       ~SPISensor(); 

    protected:
        int sendBytes(char *data, int count);
        int transceiveBytes(char *data, int count);

    private:
        static bool gpioIsInitialized;
        static int amountOfInstanzes;

        int spiHandler;
        int _spiChannel;
};

#endif