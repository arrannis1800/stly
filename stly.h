#ifndef STLY_H
#define STLY_H
#include <stdio.h>
#include <chrono>

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

	bool bEmpty() const;

	void insert(uint32_t idx, T item);

	void push_back(T item);

	void push_front(T item);

	T const erase(uint32_t idx);

	T const pop_back();

	uint32_t get_size();

	void _get_dev_info();

private:
	T* array;
	uint32_t size;
	uint32_t capacity;

	void recreate_array(uint32_t new_size);

public: 
	T& operator [](int64_t idx)
	{
		int64_t t_size = static_cast<int64_t>(size);

		if(idx < 0 && t_size + idx >= 0)
		{
			return array[t_size + idx];
		} else if (idx >= 0 && idx < t_size)
		{
			return array[idx];
		}
		throw -1;
	};
};

class StopWatch
{
public:
	void start_sw();
	void stop_sw();
	double get_microseconds();
	double get_miliseconds();
	double get_seconds();

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start_time;
	std::chrono::time_point<std::chrono::steady_clock> m_end_time;
	bool bRunning = false;
};

#ifdef STLY_IMPLEMENTATION

template <typename T>
bool SmartArray<T>::bEmpty() const
{
	return size == 0 ? true : false;
}

template <typename T>
void SmartArray<T>::insert(uint32_t idx, T item)
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
		for(uint32_t i = idx; i < size; i++)
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
T const SmartArray<T>::erase(uint32_t idx)
{
	if(bEmpty())
	{
		throw -1;
	}
	
	T temp = array[idx];
	if(idx != size)
	{
		for(uint32_t i = idx; i < size; i++)
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
uint32_t SmartArray<T>::get_size()
{
	return size;
};

template <typename T>
void SmartArray<T>::_get_dev_info()
{
	printf("INFO: array size: %d, array capacity: %d\n", size, capacity);
	for (uint32_t i = 0; i < size; i++)
	{
		printf("\tElement %d: %zu\n", i, array[i]);
	}
};

template <typename T>
void SmartArray<T>::recreate_array(uint32_t new_size)
{
	T* temp = new T[new_size];
	for (uint32_t i = 0; i < size; i++)
	{
		temp[i] = array[i];
	}
	delete[] array;
	array = temp;
	capacity = new_size;
};

void StopWatch::start_sw()
{
	bRunning = true;
	m_start_time = std::chrono::steady_clock::now();
}

void StopWatch::stop_sw()
{
	bRunning = false;
	m_end_time = std::chrono::steady_clock::now();
}

double StopWatch::get_microseconds()
{
	if (bRunning)
	{	
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_start_time).count();
	} else 
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(m_end_time - m_start_time).count();
	}
	
}

double StopWatch::get_miliseconds()
{
	return get_microseconds() / 1000.0;
}

double StopWatch::get_seconds()
{
	return get_miliseconds() / 1000.0;
}


#endif // STLY_IMPLEMENTATION
#endif //STLY_H