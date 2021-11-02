#include <iostream>
#include <string>
#include <ctime>
#include <sys/time.h>
#include "adxl345.h"
#include "influxdbstorage.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

int main(){
    std::cout << "Correct main" << std::endl;

    InfluxDBStorage storage("StudHSKe", "prosit", "wXrijWpcGjt_xjBitTOXPUOqXAxJXtfnanjIOiHklGMiEUNFZxhbeSoh-AtUt3tHLqnCz31XuwIoVhNf14CRqg==","measNr=0");

    ADXL345 lba("Sensor1", ((TimeLineStorage *) &storage), 3600, 0);

    char tt[100];
    time_t now = time(nullptr);
    auto tm_info = localtime(&now);
    strftime(tt, 100, "%Y-%m-%d_%H-%M-%S", tm_info);

    

    std::stringstream ss;
    ss << localtime(&now)->tm_year+1900 << "-" << localtime(&now)->tm_mon+1 << "-" << localtime(&now)->tm_mday << "_" << localtime(&now)->tm_hour+1 << "-" << localtime(&now)->tm_min << "-" << localtime(&now)->tm_sec;

    auto currentTime = (std::__cxx11::string)ss.str();
    std::cout << "Measurement: " << currentTime << std::endl;
    unsigned long timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
    std::cout << "Timestamp: " << timestamp << std::endl;


    lba.measure(currentTime);
}