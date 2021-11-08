#ifndef INFLUX_DB_H
#define INFLUX_DB_H

#include <string.h>
using std::string;
#include <map>
#include <thread>
#include <vector>
#include <chrono>

#include "timelineStorage.h"
#include "influxdb-cpp-2/influxdb.hpp"

class InfluxDBStorage : protected TimeLineStorage
{
    public:
        InfluxDBStorage(string organisationName, std::string bucket, std::string token, std::string tags);
       ~InfluxDBStorage(); 

        void store(const std::__cxx11::string& measurementName, const string& fieldName, const string& fieldValue);
        void store(const std::__cxx11::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields, std::chrono::_V2::system_clock::time_point timePoint) override;

    private:
        std::string _organisationName;
        std::string _bucket;
        std::string _token;
        std::string _tags;

        influxdb_cpp::server_info serverInfo;
        std::string writeCommand;

        influxdb_cpp::builder builder;
        unsigned long builderEntries;
        std::vector<std::thread> threads;
};

#endif