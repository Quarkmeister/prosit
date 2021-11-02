// global includes
#include <stdexcept>
#include <pigpio.h>
extern "C" int gpioInitialise();
extern "C" void gpioTerminate(void);
extern "C" int spiOpen(unsigned spiChan, unsigned baud, unsigned spiFlags);
extern "C" int spiWrite(unsigned handle, char *buf, unsigned count);
extern "C" int spiXfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count);

// local includes
#include "SPISensor.h"

// local settings
const int SPI_SPEED = 2'000'000; // Bauds = Symbols per second  

// local constants
#define MULTI_BIT       0x40
#define SPI_FLAGS       3
#define READ_BIT        0x80

// Initialisations
bool SPISensor::gpioIsInitialized = false;
int SPISensor::amountOfInstanzes = 0;

SPISensor::SPISensor(string identifier, TimeLineStorage* timeLineStorage, int spiChannel) : Sensor(identifier, timeLineStorage){
    _spiChannel = spiChannel;

    // Initialize the GPIO library
    if (!gpioIsInitialized)
    {
        if(gpioInitialise() < 0)
            throw std::runtime_error("gpio library couldnt be initialized");
        
        gpioIsInitialized = true;
    }

    // Arrange the SPI connection
    spiHandler = spiOpen(_spiChannel, SPI_SPEED, SPI_FLAGS);
    if(spiHandler < 0)
        throw std::runtime_error("SPI couldnt be opened!");

    // This is important because of the static gpio initialisation
    ++amountOfInstanzes;
}
SPISensor::~SPISensor(){
    if(--amountOfInstanzes == 0)
        gpioTerminate();
} 

int SPISensor::sendBytes(char *data, int count){
    if (count > 1) data[0] |= MULTI_BIT;
    return spiWrite(spiHandler, data, count);
}
int SPISensor::transceiveBytes(char *data, int count){
    data[0] |= READ_BIT;
    if (count > 1) data[0] |= MULTI_BIT;
    return spiXfer(spiHandler, data, data, count);
} 
