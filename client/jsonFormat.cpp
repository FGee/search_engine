#include "jsonFormat.h"

#include <json/json.h>

namespace wd
{

string JsonFormat::dataToJson(const string& data) 
{
    Json::FastWriter writerInfo;

    Json::Value writerValueInfo;

    writerValueInfo["QueryWord"] = data.c_str();

    return writerInfo.write(writerValueInfo);
}

}
