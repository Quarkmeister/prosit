#include <iostream>
#include <string>
#include <ctime>
#include <sys/time.h>
#include "adxl345.h"
#include "influxdbstorage.h"

int main(){
    std::cout << "Correct main" << std::endl;

    InfluxDBStorage storage("StudHSKe", "prosit", "wXrijWpcGjt_xjBitTOXPUOqXAxJXtfnanjIOiHklGMiEUNFZxhbeSoh-AtUt3tHLqnCz31XuwIoVhNf14CRqg==","measNr=0");

    ADXL345 lba("Sensor1", ((TimeLineStorage *) &storage), 0);

    char tt[100];
    time_t now = time(nullptr);
    auto tm_info = localtime(&now);
    strftime(tt, 100, "%Y-%m-%d_%H-%M-%S", tm_info);

    std::cout << "Measurement: " << string(tt, 19) << std::endl;

    lba.measure(string(tt, 19));
}