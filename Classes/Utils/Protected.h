#ifndef _UTILS_PROTECTED_H_
#define _UTILS_PROTECTED_H_

#include <algorithm>

template<typename T, size_t S = 3> class Protected
{
private:
    T _secret;
    T _values[S];
    
public:
    Protected(T value = 0);
    T getValue();
    void setValue(T value);
};

template<typename T, size_t S> Protected<T, S>::Protected(T value)
{
    _secret = rand();
    std::fill(_values, _values + S, 0);
    setValue(value);
}

template<typename T, size_t S> T Protected<T, S>::getValue()
{
    T sum = 0;
    for (size_t i = 0; i < S; i++)
    {
        sum += _values[i] ^ _secret;
    }
    return sum;
}

template<typename T, size_t S> void Protected<T, S>::setValue(T value)
{
    size_t index = rand() % S;
    T partialSum = 0;
    for (size_t i = 0; i < S; i++)
    {
        if (i != index)
        {
            partialSum += _values[i] ^ _secret;
        }
    }
    _values[index] = (value - partialSum) ^ _secret;
}


#endif
