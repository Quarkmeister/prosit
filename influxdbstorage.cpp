#include <stdexcept>
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
#include <cmath>
#include <iostream>
#include <string>
using std::string;

#include "influxdbstorage.h"

InfluxDBStorage::InfluxDBStorage(string organisationName, string bucket, string token, string tags) 
    : serverInfo("127.0.0.1", 8086, organisationName, token, bucket), storeThread{}{

    builder_work = new influxdb_cpp::builder{};
    builderInitialised = false;
}

InfluxDBStorage::~InfluxDBStorage(){

    try {
        // Waits till the thread finishes
        storeThread.get();

        // Push the unstored Data
        pushData(serverInfo, *builder_work);
    }
    catch(const std::exception& e) {
        delete builder_work;
        builder_work = nullptr;

        delete builder_store;
        builder_store = nullptr;

        //throw e;
    }
    delete builder_work;
    builder_work = nullptr;

    delete builder_store;
    builder_store = nullptr;
}

void InfluxDBStorage::store(const std::__cxx11::string& measurementName, const string& fieldName, const string& fieldValue){

    std::map<string, string> tag;

    std::map<string, string> field;
    field[fieldName]  = fieldValue;

    auto timePointNow = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now());

    // garantees a polymorphic call
    this->store(measurementName, tag, field, timePointNow);
} 

void InfluxDBStorage::store(const std::__cxx11::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields, std::chrono::_V2::system_clock::time_point timePoint) {
    
    // Converts the timepoint parameter to a unix timestamp
    unsigned long long timestamp = duration_cast<nanoseconds>(timePoint.time_since_epoch()).count();

    // Sets the measurement value
    influxdb_cpp::detail::tag_caller* tagCaller;
    if (!builderInitialised) {

        // This steps garuantees with polymorphy, that a new batch write is started.
        tagCaller = &((builder_work)->meas(measurementName));
        builderInitialised = true;
    }
    else {
        tagCaller = &(((influxdb_cpp::detail::field_caller*)(builder_work))->meas(measurementName));
    }

    // Sets the tag values
    for (auto it = tags.begin(); it != tags.end(); it++)
    {
        tagCaller->tag(it->first, it->second);
    }

    // Sets the field values
    influxdb_cpp::detail::field_caller* fieldCaller;
    bool firstRun = true;
    for (auto it = fields.begin(); it != fields.end(); ++it) {
        if (firstRun) {
            fieldCaller = &tagCaller->field(it->first, it->second);
            firstRun = false;
        }
        else {
            fieldCaller->field(it->first, it->second);
        }
    }

    // Sets a timestamp. This is important because of the batch writing. Otherwise all measurement points will get the same timepoint
    // from the database routines.
    fieldCaller->timestamp(timestamp);

    // Check if the store thread finished it´s work. If it´s finished a new thread is initalized and a new builder_work 
    // is been created.
    if (storeThread.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {

        builder_store = builder_work;
        renewBuilderWork();

        // std::launch::async   : The thread starts immideatly
        // std::ref             : The parameters would be copied otherwise
        // true                 : The 'builder_store' pointer will be deleted from the thread after function run
        storeThread = std::async(std::launch::async, &pushData, std::ref(serverInfo), std::ref(builder_store), true);
    }
}

void InfluxDBStorage::pushData(influxdb_cpp::server_info& serverInfo, influxdb_cpp::builder& builder, bool deleteBuilder) {

    ((influxdb_cpp::detail::field_caller*)&builder)->post_http(serverInfo);

    if (deleteBuilder)
        delete builder;
}

void InfluxDBStorage::renewBuilderWork() {
    builder_work = new influxdb_cpp::builder{};
    builderInitialised = false;
}