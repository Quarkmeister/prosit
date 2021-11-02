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

    // std::cout << "Threads will be deleted" << std::endl;
    // for(auto &thread: threads){
    //     if(thread.joinable())
    //         thread.join();
    // }
    ((influxdb_cpp::detail::field_caller*)&builder)->post_http(serverInfo);

} 

void InfluxDBStorage::store(const std::__cxx11::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields){

    unsigned long long timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();

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

    // if(++builderEntries > builderEntriesBufferLength){
        
    //     auto thread = std::thread{InfluxDBStorage::transmittBuffer, builder, serverInfo};
    //     threads.push_back(thread);
    //     thread.detach();

    //     builder = influxdb_cpp::builder();
    // }
}
void InfluxDBStorage::transmittBuffer(influxdb_cpp::builder builder, influxdb_cpp::server_info serverInfo){

    std::string response;

    ((influxdb_cpp::detail::field_caller*)&builder)->post_http(serverInfo, &response);

    std::cout << "Response: " << response << std::endl;

} 