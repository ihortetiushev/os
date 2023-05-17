#include "pch.h"
#include "Buffer.h"

template<class T>
void Buffer<T>::push(T dataItem) 
{
	lock.lock();
	queue.push(dataItem);
	lock.unlock();
}

template<class T>
T Buffer<T>::get()
{
	lock.lock();
	int val = queue.front();
	queue.pop();
	lock.unlock();
	return val;
}

template<class T>
size_t Buffer<T>::getSize()
{
	lock.lock();
	size_t sz =  queue.size();
	lock.unlock();
	return sz;
}

template<class T>
std::vector<T> Buffer<T>::getAll()
{
	lock.lock();
	std::vector<T> data;
	while (!queue.empty) 
	{
		data.push_back(queue.pop);
	}
	lock.unlock();
	return data;
}

template<class T>
void Buffer<T>::putAll(std::vector<T> data) 
{
	lock.lock();
	for (auto item : data) 
	{
		queue.push(item);
	}
	lock.unlock();
}
