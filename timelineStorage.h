#ifndef TIME_LINE_STORAGE_H
#define TIME_LINE_STORAGE_H

#include <string>
using std::string;
#include <vector>
#include <map>
#include <chrono>

class TimeLineStorage
{
    public:
        virtual void store(const std::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields, std::chrono::_V2::system_clock::time_point timePoint) = 0;
};

#endif