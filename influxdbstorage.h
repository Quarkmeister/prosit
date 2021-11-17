#ifndef INFLUX_DB_H
#define INFLUX_DB_H

#include <string.h>
using std::string;
#include <map>
#include <thread>
#include <future>
#include <vector>
#include <chrono>

#include "timelineStorage.h"
#include "influxdb-cpp-2/influxdb.hpp"

class InfluxDBStorage : protected TimeLineStorage
{
    public:
        InfluxDBStorage(string organisationName, std::string bucket, std::string token, std::string tags);
       ~InfluxDBStorage(); 
        
       // Both store functions work asynchron.
       void store(const std::__cxx11::string& measurementName, const string& fieldName, const string& fieldValue);
       void store(const std::__cxx11::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields, std::chrono::_V2::system_clock::time_point timePoint) override;

    private:
        influxdb_cpp::server_info serverInfo;

        influxdb_cpp::builder* builder_work;
        influxdb_cpp::builder* builder_store;
        bool builderInitialised;
        void renewBuilderWork();

        std::future<void> storeThread;
        static void pushData(influxdb_cpp::server_info serverInfo, influxdb_cpp::builder* builder, bool deleteBuilder = false);
};

#endif