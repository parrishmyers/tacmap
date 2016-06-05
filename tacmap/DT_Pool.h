#pragma once

#include <stdio.h>
#include "DT_Triangle.h"

template<class T, int Size>
class Pool {
private:
	T * t;
	int numT;
public:
	Pool();
	~Pool();
	int len();
	T * get();
	T * operator[] (int const i);
};

template<class T, int Size>
Pool<T,Size>::Pool()
{
	t = new T[Size];
	numT = 0;
}

template<class T, int Size>
Pool<T, Size>::~Pool()
{
	if (t != nullptr) 
		delete[] t;
}

template<class T, int Size>
int Pool<T, Size>::len()
{
	return numT;  
}

template<class T, int Size>
T * Pool<T, Size>::get()
{
	if (numT < Size) {
		return &t[numT++];
	}
	else {
		return nullptr;
	}
}

template<class T, int Size>
T * Pool<T, Size>::operator[] (int const i)
{
	if (0 <= i && i < numT) {
		return &t[i];
	}
	else {
		fprintf(stdout, "WARNING: Out of range of Pool contents!\n");
		return nullptr;
	}
}
