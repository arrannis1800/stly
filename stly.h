#ifndef STLY_H
#define STLY_H
#include <stdio.h>
#include <chrono>
#include <map>
#include <initializer_list>

template <typename T> 
class SmartArray
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

template <typename T>
class Matrix
{
public:
	Matrix(std::initializer_list<T> init_list, size_t cols_, size_t rows_)
	{
		cols = cols_;
		rows = rows_;
		if(cols_*rows_ != init_list.size())
		{
			printf("ERROR: Provided initializer list length does not match SPRITE_LEN.\n");
			return;
		}

		arr = new T[cols*rows];
		size_t c = 0, r = 0;
		for (auto& elem : init_list)
		{
			arr[r*cols + c]=elem;
			c++;
			if (c >= cols)
			{
				c = 0;
				r++;
			}
		}		
	};

	~Matrix()
	{
		delete[] arr;
	};

	T* operator[](int i)
	{
		return arr[i];
	};

	size_t get_rows() const;

	size_t get_cols() const;

	void rotate();

private:
	size_t rows;
	size_t cols;
	T* arr;
};

template<typename K, typename V>
class IntervalMap {
public:
	// constructor associates whole range of K with val
	IntervalMap(V const& val)
	: m_valBegin(val)
	{}
	void assign( K const& keyBegin, K const& keyEnd, V const& val );

	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
private:
	V m_valBegin;
	std::map<K,V> m_map;
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
size_t Matrix<T>::get_rows() const
{
	return rows;
};

template <typename T>
size_t Matrix<T>::get_cols() const
{
	return cols;
};

template<typename T>
void Matrix<T>::rotate()
{
	for(int i=0; i<rows; i++)
	{
		for(int j=i+1; j<cols; j++)
			std::swap(arr[i][j], arr[j][i]);
	}

	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols/2; j++)
		{
			std::swap(arr[i][j], arr[i][cols-j-1]);
		}
	}
};

template<typename K, typename V>
void IntervalMap<K, V>::assign( K const& keyBegin, K const& keyEnd, V const& val )
{
	if (keyBegin < keyEnd)
	{
		V temp_v = this->operator[](keyBegin);
		V temp_v_prev = m_map.lower_bound(keyBegin) == m_map.begin() ? m_valBegin : (--m_map.lower_bound(keyBegin))->second;
		if(!(temp_v == val) && temp_v_prev == val) 
		{
			m_map[keyEnd] = temp_v;
			m_map.erase(keyBegin);
		} else if (!(temp_v == val) && !(temp_v_prev == val))
		{
			m_map[keyBegin] = val;
			m_map[keyEnd] = temp_v;
		}

		

		V temp_vv = m_map.lower_bound(keyEnd) == m_map.begin() ? m_valBegin : (--m_map.lower_bound(keyEnd))->second;
		while (!((m_map.lower_bound(keyEnd) == m_map.begin() ? m_valBegin : (--m_map.lower_bound(keyEnd))->second) == this->operator[](keyBegin)))
		{
			auto it=m_map.lower_bound(keyEnd);
			if(it==m_map.begin()) {
				break;
			} else {
				if (m_map.upper_bound(keyEnd) != m_map.end() )
				{
					m_map[keyEnd] = temp_vv;
				}
				m_map.erase((--it)->first);
			}
		};
	}
}	

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