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

bool IsSlowMem(const void* pAddr, uint uiAmount)
{
	DWORD uiAddr = ((DWORD)pAddr) >> 8;
	DWORD uiAddrLast = (((DWORD)pAddr) + uiAmount - 1) >> 8;

	for (; uiAddr <= uiAddrLast; uiAddr++)
	{
		if (uiAddr < 0x4010 || uiAddr > 0x8A40 || uiAddr == 0x4C03 || uiAddr == 0x4EB9 || uiAddr == 0x5022 || uiAddr == 0x5023 || uiAddr == 0x50AB ||
			uiAddr == 0x50BF || uiAddr == 0x5332 || uiAddr == 0x609C || uiAddr == 0x60D8 || uiAddr == 0x60F2 || uiAddr == 0x6420 || uiAddr == 0x648A ||
			uiAddr == 0x64CA || uiAddr == 0x6870 || uiAddr == 0x6A07 || uiAddr == 0x6AEA || uiAddr == 0x729B || uiAddr == 0x742B)
			return false;
	}
	return true;
}
