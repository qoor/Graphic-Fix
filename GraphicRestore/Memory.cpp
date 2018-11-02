#include "stdafx.h"

#include "Memory.h"

void MemCopy(void* ptr, const void* src, int length)
{
	memcpy(ptr, src, length);
}

void MemSet(void* ptr, int value, int amount)
{
	memset(ptr, value, amount);
}
