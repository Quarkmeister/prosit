#include <stdexcept>
#include <chrono>

#include "influxdbstorage.h"
using std::string;

const unsigned long builderEntriesBufferLength = 100;


InfluxDBStorage::InfluxDBStorage(std::string organisationName, std::string bucket, std::string token, std::string tags) 
: serverInfo("127.0.0.1", 8086, organisationName, token, bucket), builder{}, threads{} {

    _organisationName = organisationName;
    _bucket = bucket;
    _token = token;
    _tags = tags;

    writeCommand = "influx write -o " + _organisationName + " -b " + _bucket + " -t " + _token;

    builderEntries = 0;
}

InfluxDBStorage::~InfluxDBStorage(){

    ((influxdb_cpp::detail::field_caller*)&builder)->post_http(serverInfo);
} 

void InfluxDBStorage::store(const std::__cxx11::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields, std::chrono::_V2::system_clock::time_point timePoint){

    unsigned long long timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(timePoint.time_since_epoch()).count();

    auto tagsIterator = tags.rbegin();
    auto fieldsIterator = fields.rbegin();

    influxdb_cpp::detail::tag_caller* tagCaller;    
    if(builderEntries++ == 0)
        tagCaller = &((&builder)->meas(measurementName));
    else{
        tagCaller = &(((influxdb_cpp::detail::field_caller*)(&builder))->meas(measurementName));
    } 

    for (auto it = tags.begin(); it != tags.end(); it++)
    {
        tagCaller->tag(it->first, it->second);
    }

    influxdb_cpp::detail::field_caller* fieldCaller;
    bool firstRun = true;
    for(auto it = fields.begin(); it != fields.end(); ++it){
        if(firstRun){
            fieldCaller = &tagCaller->field(it->first, it-> second);
            firstRun = false;
        }
        else{
            fieldCaller->field(it->first, it->second);
        }
    }
    fieldCaller->timestamp(timestamp);
}