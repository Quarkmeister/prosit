#ifndef INFLUX_DB_H
#define INFLUX_DB_H

#include <string.h>
using std::string;
#include <map>

#include "timelineStorage.h"
#include "influxdb-cpp-2/influxdb.hpp"

class InfluxDBStorage : protected TimeLineStorage
{
    public:
        InfluxDBStorage(string organisationName, std::string bucket, std::string token, std::string tags);

        void store(const std::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields) override;

    private:
        std::string _organisationName;
        std::string _bucket;
        std::string _token;
        std::string _tags;

        influxdb_cpp::server_info serverInfo;
        std::string writeCommand;
};

#endif