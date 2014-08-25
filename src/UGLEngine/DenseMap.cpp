#include "DenseMap.h"

template <typename ValueType>
DenseMap<ValueType>::DenseMap()
{
	holes = 0;
	vector = std::vector<ValueType*>();
}

template <typename ValueType>
DenseMap<ValueType>::~DenseMap()
{
	delete vector;
}

//WARNING: Do not add() null pointers!
template <typename ValueType>
unsigned int DenseMap<ValueType>::add(ValueType* value)
{
	if (holes == 0)
	{
		vector.push_back(value);
		return vector.size() - 1;
	}
	else
	{
		for (unsigned int i = 0; i < vector.size(); i++)
		{
			if (vector[i] == null)
			{
				vector[i] = value;
				holes--;
			}
		}
	}
}

template <typename ValueType>
void DenseMap<ValueType>::remove(unsigned int index)
{
	//removing from end
	if (index == vector.size() - 1) 
		vector.pop_back();
	else
	{
		vector[i] = null;
		holes++;
	}
}

template <typename ValueType>
ValueType* DenseMap<ValueType>::get(unsigned int index)
{
	return vector[index];
}

template <typename ValueType>
ValueType* DenseMap<ValueType>::operator[](unsigned int index)
{
	return vector[index];
}

template <typename ValueType>
unsigned int DenseMap<ValueType>::numValues()
{
	return vector.size() - holes;
}