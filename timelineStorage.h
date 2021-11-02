#ifndef TIME_LINE_STORAGE_H
#define TIME_LINE_STORAGE_H

#include <string>
using std::string;
#include <vector>
#include <map>

class TimeLineStorage
{
    public:
        virtual void store(const std::string& measurementName, const std::map<string, string>& tags, const std::map<string, string>& fields) = 0;
};

#endif