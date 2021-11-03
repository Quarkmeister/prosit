#include <iostream>
#include <string>
#include <ctime>
#include <sys/time.h>
#include "adxl345.h"
#include "influxdbstorage.h"
//#include <argparse/include/argparse/argparse.hpp> //https://github.com/p-ranav/argparse
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using std::chrono::time_point_cast;
using std::chrono::seconds;

bool process_command_line(int argc, char** argv, float& timeSpan);

int main(int argc, char *argv[]){

    // Get the sample Time as an input from the command line
    float sampleTime;

    bool result = process_command_line(argc, argv, sampleTime);
    if (!result)
        return 1;

    // Instanziate the timeline storage. Can handle multiple sensors.
    InfluxDBStorage storage("StudHSKe", "prosit", "wXrijWpcGjt_xjBitTOXPUOqXAxJXtfnanjIOiHklGMiEUNFZxhbeSoh-AtUt3tHLqnCz31XuwIoVhNf14CRqg==","measNr=0");

    // Instanziate the sensors.
    ADXL345 lba("Sensor1", ((TimeLineStorage *) &storage), 3200, 0);


    // Print the measure identifier
    time_t now = time(nullptr);
    auto tm_info = localtime(&now);    

    std::stringstream ss;
    ss << localtime(&now)->tm_year+1900 << "-" << localtime(&now)->tm_mon+1 << "-" << localtime(&now)->tm_mday << "_" << localtime(&now)->tm_hour+1 << "-" << localtime(&now)->tm_min << "-" << localtime(&now)->tm_sec;

    auto measurementIdentifier = (std::__cxx11::string)ss.str();
    std::cout << "Measurement: " << measurementIdentifier << std::endl;


    // Measuring procedure
    const clock_t begin_time = clock();
    while(float( clock () - begin_time ) /  CLOCKS_PER_SEC < sampleTime){

        lba.measure(measurementIdentifier);

    }
}

bool process_command_line(int argc, char** argv, float& timeSpan){

    try
    {
        po::options_description desc{"Options"};
        desc.add_options()
          ("help,h",     "produce help message")
          ("time-span,t",   po::value<float>(&timeSpan)->required(),      "Set the sample time span in seconds")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return false;
        }

        // There must be an easy way to handle the relationship between the
        // option "help" and "host"-"port"-"config"
        // Yes, the magic is putting the po::notify after "help" option check
        po::notify(vm);
    }
    catch(std::exception& e)
    {
        throw e;
    }

    return true;
}