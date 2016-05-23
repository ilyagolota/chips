#ifndef __ChipsX__HttpUtility__
#define __ChipsX__HttpUtility__

#include <string>
#include <map>

class HttpUtility
{
public:
    static std::string urlEncode(const std::string& str);
    static std::string urlEncode(const char* chars, size_t length);
    static std::string urlDecode(const std::string& str);
    static std::string urlDecode(const char* chars, size_t length);
    static std::map<std::string, std::string> parseQueryString(const std::string& query);
};

#endif
