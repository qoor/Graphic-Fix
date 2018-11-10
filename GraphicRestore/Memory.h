#pragma once

#include <iostream>

typedef unsigned int uint;

void MemCopy(void* ptr, const void* src, int length);
void MemSet(void* ptr, int value, int amount);
bool IsSlowMem(const void* pAddr, uint uiAmount);

template <class T>
void MemPut(int ptr, const T value, int size = sizeof(T))
{
	void* pointer = reinterpret_cast<void*>(ptr);

	if (*(static_cast<T*>(pointer)) != value)
		memcpy(pointer, &value, size);
}

inline void MemCopyFast(void* ptr, const void* src, int length)
{
	memcpy(ptr, src, length);
}

inline void MemSetFast(void* ptr, int value, int amount)
{
	memset(ptr, value, amount);
}

template <class T, class U>
void MemPutFast(U ptr, const T value)
{
	*(T*)ptr = value;
}
