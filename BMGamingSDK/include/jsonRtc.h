

#ifndef MAPLE_RTC_BASE_JSON_H_
#define MAPLE_RTC_BASE_JSON_H_

#include <string>
#include <vector>


#include "./json/json.h"


namespace rtc {

///////////////////////////////////////////////////////////////////////////////
// JSON Helpers
///////////////////////////////////////////////////////////////////////////////

 extern "C" __attribute__((visibility("default"))) bool JsonArrayToStringVector(const Json::Value& in,
                             std::vector<std::string>* out);

}  // namespace rtc

#endif  
