#include "BinaryWriter.h"

using namespace utils;

BinaryWriter::BinaryWriter(std::ostream* stream)
{
    _stream = stream;
}

void BinaryWriter::writeUInt8(unsigned char value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeUInt16(unsigned short int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeUInt32(unsigned int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeUInt64(unsigned long int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeInt8(char value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeInt16(short int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeInt32(int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeInt64(long long int value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeFloat(float value)
{
    _stream->write((char *)&value, sizeof value);
}

void BinaryWriter::writeDouble(double value)
{
    _stream->write((char *)&value, sizeof value);
}

std::ostream& BinaryWriter::getBaseStream()
{
    return *_stream;
}
