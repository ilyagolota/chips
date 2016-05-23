#include "BinaryReader.h"

using namespace utils;

BinaryReader::BinaryReader(std::istream *stream)
{
    _stream = stream;
}

unsigned char BinaryReader::readUInt8()
{
    unsigned char value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

unsigned short int BinaryReader::readUInt16()
{
    unsigned short int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

unsigned int BinaryReader::readUInt32()
{
    unsigned int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

unsigned long long int BinaryReader::readUInt64()
{
    unsigned long long int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

char BinaryReader::readInt8()
{
    char value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

short int BinaryReader::readInt16()
{
    short int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

int BinaryReader::readInt32()
{
    int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

long long int BinaryReader::readInt64()
{
    long long int value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

float BinaryReader::readFloat()
{
    float value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

double BinaryReader::readDouble()
{
    double value;
    _stream->read((char *)&value, sizeof value);
    return value;
}

void BinaryReader::read(char* bytes, int count)
{
    _stream->read(bytes, count);
}

std::istream& BinaryReader::getBaseStream()
{
    return *_stream;
}
