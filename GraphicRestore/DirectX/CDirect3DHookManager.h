#pragma once

#include "CDirect3DHook9.h"

class CDirect3DHookManager
{
public:
	CDirect3DHookManager();
	~CDirect3DHookManager();

	void ApplyHook();
	void RemoveHook();

private:
	CDirect3DHook9* m_pDirect3DHook9;
};
