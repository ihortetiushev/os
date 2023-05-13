#pragma once
#include <queue>
#include <mutex>

template<class T>
class Buffer
{

private:
	std::mutex lock;
	std::queue<T> queue;

public:
	void push(T dataItem);
	T get();
	size_t getSize();
	std::vector<T> getAll();
	void putAll(std::vector<T> data);
};

