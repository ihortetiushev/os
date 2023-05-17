#pragma once
#include <queue>

template<class T>
class Buffer
{

private:
	std::queue<T> queue;

public:
	void push(T dataItem);
	T get();
	size_t getSize();
	std::vector<T> getAll();
	void putAll(std::vector<T> data);
};

