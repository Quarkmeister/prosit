#include <stdexcept>

#include "influxdbstorage.h"
using std::string;


InfluxDBStorage::InfluxDBStorage(std::string organisationName, std::string bucket, std::string token, std::string tags) : serverInfo("127.0.0.1", 8086, organisationName, token, bucket){
    _organisationName = organisationName;
    _bucket = bucket;
    _token = token;
    _tags = tags;

    writeCommand = "influx write -o " + _organisationName + " -b " + _bucket + " -t " + _token;
} 

void InfluxDBStorage::store(const std::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields){

    auto tagsIterator = tags.rbegin();
    auto fieldsIterator = fields.rbegin();

    switch (tags.size())
    {
    case 0:
        switch (fields.size())
        {
        case 0:
            throw std::runtime_error("At least one field is needed.");
        
        default:
            throw std::runtime_error("There is no implementation for the needed amount of fields.");
        }
        break;
    case 1:
        switch (fields.size())
        {
        case 0:
           throw std::runtime_error("At least one field is needed.");

        case 3:
            influxdb_cpp::builder()
                .meas(measurementName)
                .tag(tagsIterator->first, tagsIterator->second)
                .field(fieldsIterator->first, fieldsIterator->second)
                .field((++fieldsIterator)->first, fieldsIterator->second)
                .field((++fieldsIterator)->first, fieldsIterator->second)
                .post_http(serverInfo);
            break;
        
        default:
            throw std::runtime_error("There is no implementation for the needed amount of fields.");
        }
        break;
    
    default:
        throw std::runtime_error("There is no implementation for the needed amount of tags.");
    }
} 