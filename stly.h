#ifndef STLY_H
#define STLY_H
#include <stdio.h>

template <typename T> class SmartArray
{
public:
	SmartArray()
	{
	size = 0;
	capacity = 32;
	array = new T[capacity];
	};

	~SmartArray()
	{
		delete[] array;
	};

	bool const bEmpty();

	void insert(size_t idx, T item);

	void push_back(T item);

	void push_front(T item);

	T const erase(size_t idx);

	T const pop_back();

	size_t get_size();

	void _get_dev_info();

private:
	T* array;
	size_t size;
	size_t capacity;

	void recreate_array(size_t new_size);

public: 
	T& operator [](size_t idx)
	{
		if (idx < size)
		{
			return array[idx];
		}
		throw -1;
	};
};

#ifdef STLY_IMPLEMENTATION

template <typename T>
bool const SmartArray<T>::bEmpty()
{
	return size == 0 ? true : false;
}

template <typename T>
void SmartArray<T>::insert(size_t idx, T item)
{
	if(size >= capacity)
	{
		recreate_array(capacity*2);
	}

	if(idx == size)
	{
		array[size] = item;
	} else
	{
		T temp = item;
		for(size_t i = idx; i < size; i++)
		{
			temp += array[i];
			array[i] = temp - array[i];
			temp = temp - array[i];
		}
		array[size] = temp;
	}
	size += 1;
}

template <typename T>
void SmartArray<T>::push_back(T item)
{
	insert(size, item);
};

template <typename T>
void SmartArray<T>::push_front(T item)
{
	insert(0, item);
};

template <typename T>
T const SmartArray<T>::erase(size_t idx)
{
	if(bEmpty())
	{
		throw -1;
	}
	
	T temp = array[idx];
	if(idx != size)
	{
		for(size_t i = idx; i < size; i++)
		{
			array[i] = array[i+1];
		}
	}

	size -= 1;
	if(size < capacity/4)
	{
		recreate_array(capacity/2);
	}
	return temp;
}

template <typename T>
T const SmartArray<T>::pop_back()
{ 	
	return erase(size);
};

template <typename T>
size_t SmartArray<T>::get_size()
{
	return size;
};

template <typename T>
void SmartArray<T>::_get_dev_info()
{
	printf("INFO: array size: %d, array capacity: %d\n", size, capacity);
	for (size_t i = 0; i < size; i++)
	{
		printf("\tElement %d: %zu\n", i, array[i]);
	}
};

template <typename T>
void SmartArray<T>::recreate_array(size_t new_size)
{
	T* temp = new T[new_size];
	for (size_t i = 0; i < size; i++)
	{
		temp[i] = array[i];
	}
	delete[] array;
	array = temp;
	capacity = new_size;
};

#endif // STLY_IMPLEMENTATION
#endif //STLY_H