#ifndef _UTILS_BINARY_READER_
#define _UTILS_BINARY_READER_

#include <string>
#include <iostream>
#include <cocos2d.h>

namespace utils
{
    class BinaryReader
    {
    public:
        BinaryReader(std::istream* stream);
        unsigned char readUInt8();
        unsigned short int readUInt16();
        unsigned int readUInt32();
        unsigned long long int readUInt64();
        char readInt8();
        short int readInt16();
        int readInt32();
        long long int readInt64();
        float readFloat();
        double readDouble();
        void read(char* bytes, int count);
        std::istream &getBaseStream();
    protected:
        std::istream *_stream;
    };
}

#endif
