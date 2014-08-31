#pragma once

#include <vector>

template <typename ValueType>
class DenseMap
{
public:
    std::vector<ValueType*> vector;
    unsigned int holes;
    
    DenseMap();
    ~DenseMap();
    
    unsigned int add(ValueType* value);
    
    void remove(unsigned int index);
    
    ValueType* get(unsigned int index);
    
    ValueType* operator[](unsigned int index);
    
    unsigned int numValues();
};