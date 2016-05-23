#include "HttpUtility.h"

std::string HttpUtility::urlEncode(const std::string& str)
{
    return urlEncode(&str[0], str.length());
}

std::string HttpUtility::urlEncode(const char* chars, size_t length)
{
    std::string encodedString;
    
    char hexBuffer[10];
    
    for(size_t i = 0; i < length; i++)
    {
        char c = chars[i];
        if (c == ' ')
        {
            encodedString += '+';
        }
        else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encodedString += c;
        }
        else
        {
            sprintf(hexBuffer, "%%%2x", c);
            encodedString += hexBuffer;
        }
    }
    return encodedString;
}

std::string HttpUtility::urlDecode(const std::string& str)
{
    return urlDecode(&str[0], str.length());
}

std::string HttpUtility::urlDecode(const char* chars, size_t length)
{
    std::string decodedString;
    
    for (size_t i = 0; i < length; i++)
    {
        if (chars[i] != '%')
        {
            decodedString += chars[i];
        }
        else
        {
            int hexCode;
            sscanf(&chars[i + 1], "%2x", &hexCode);
            decodedString += static_cast<char>(hexCode);
            i += 2;
        }
    }
    return decodedString;
}

std::map<std::string, std::string> HttpUtility::parseQueryString(const std::string& query)
{
    std::map<std::string, std::string> params;
    
    int length = query.length();
    const char* chars = &query[0];
    
    int i = 0;
    if (chars[0] == '?')
    {
        i += 1;
    }
    
    while (i < length)
    {
        int startIndex = i;
        int equalsIndex = std::string::npos;
        
        while (i < length)
        {
            char c = chars[i];
            if (c == '=')
            {
                if (equalsIndex == std::string::npos)
                {
                    equalsIndex = i;
                }
            }
            else if (c == '&')
            {
                break;
            }
            i++;
        }
        
        if (equalsIndex != std::string::npos)
        {
            auto name = urlDecode(&chars[startIndex], equalsIndex - startIndex);
            auto value = urlDecode(&chars[equalsIndex + 1], i - equalsIndex - 1);
            params.insert(std::make_pair(std::move(name), std::move(value)));
        }
        else
        {
            auto name = urlDecode(&chars[startIndex], i - startIndex);
            params.insert(std::make_pair(std::move(name), ""));
        }
        
        if (i == length - 1 && chars[i] == '&')
        {
            params.insert(std::make_pair("", ""));
        }
        
        i++;
    }
    
    return params;
}
