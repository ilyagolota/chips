#ifndef _UTILS_HASH_ALGORITHM_
#define _UTILS_HASH_ALGORITHM_

#include <vector>
#include <string>
#include <iostream>

namespace utils
{
    class HashAlgorithm
    {
    protected:
        int _hashSize;
        int _state;
        char* _hashValue;
        
    public:
        HashAlgorithm();
        int getHashSize();
        void update(std::istream* inputStream);
        void update(std::string& string);
        void update(const char* buffer, int length);
        std::vector<char> getDigest();
        
    protected:
        virtual void _hashCore(const char* buffer, int length) = 0;
        virtual char *_hashFinal() = 0;
    };
}

#endif
