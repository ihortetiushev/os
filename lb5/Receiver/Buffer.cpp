#include "pch.h"
#include "Buffer.h"

template<class T>
void Buffer<T>::push(T dataItem) 
{
	queue.push(dataItem);
}

template<class T>
T Buffer<T>::get()
{
	T val = queue.front();
	queue.pop();
	return val;
}

template<class T>
size_t Buffer<T>::getSize()
{
	size_t sz =  queue.size();
	return sz;
}

template<class T>
std::vector<T> Buffer<T>::getAll()
{
	std::vector<T> data;
	while (!queue.empty()) 
	{
		data.push_back(this->get());
	}
	return data;
}

template<class T>
void Buffer<T>::putAll(std::vector<T> data) 
{
	for (auto item : data) 
	{
		queue.push(item);
	}
}
