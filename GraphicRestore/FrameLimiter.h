#pragma once

#include "HookSystem.h"
#include "TimerChecker.h"
#include "GraphicRestore.h"

extern GraphicRestore* gr;

class FrameLimiter
{
private:
	TimeChecker frameRateTimer;

public:
	FrameLimiter();
	void OnGametimerUpdate();
	void ApplyQueuedFrameRateLimit();
	int Test() { return 1;  }
};
