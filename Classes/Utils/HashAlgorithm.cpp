#include "HashAlgorithm.h"
#include <iostream>

using namespace utils;

HashAlgorithm::HashAlgorithm()
{
    _state = 0;
}

int HashAlgorithm::getHashSize()
{
    return _hashSize;
}

void HashAlgorithm::update(const char* buffer, int length)
{
    this->_hashCore(buffer, length);
}

void HashAlgorithm::update(const std::string& string)
{
    this->_hashCore(string.data(), string.size());
}

void HashAlgorithm::update(const std::vector<char>& buffer)
{
    this->_hashCore(buffer.data(), buffer.size());
}

void HashAlgorithm::update(std::istream& inputStream)
{
    char* buffer = new char[4096];
    std::streamsize bytesRead;
    do
    {
        bytesRead = inputStream.read(buffer, 4096).gcount();
        if (bytesRead > 0)
        {
            this->_hashCore(buffer, bytesRead);
        }
    } while (bytesRead > 0);
    delete[] buffer;
    inputStream.clear();
}

std::vector<char> HashAlgorithm::getDigest()
{
    if (_state == 0)
    {
        _hashValue = this->_hashFinal();
        _state = 1;
    }
    
    std::vector<char> result(_hashSize);
    memcpy(&result[0], _hashValue, _hashSize);
    return result;
}
