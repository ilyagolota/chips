//
//  Buffer.h
//  ChipsX
//
//  Created by Ilya Golota on 21.11.14.
//
//  Purpose: A Stream whose backing store is memory.  Great
//  for temporary storage without creating a temp file.  Also
//  lets users expose a void* as a stream.
//

#ifndef _UTILS_BINARY_WRITER_
#define _UTILS_BINARY_WRITER_

#include <string>
#include <iostream>

namespace utils
{
    class BinaryWriter
    {
    public:
        BinaryWriter(std::ostream* stream);
        void writeUInt8(unsigned char value);
        void writeUInt16(unsigned short int value);
        void writeUInt32(unsigned int value);
        void writeUInt64(unsigned long int value);
        void writeInt8(char value);
        void writeInt16(short int value);
        void writeInt32(int value);
        void writeInt64(long long int value);
        void writeFloat(float value);
        void writeDouble(double value);
        std::ostream &getBaseStream();
    protected:
        std::ostream* _stream;
    };
}

#endif
