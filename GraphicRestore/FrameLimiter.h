#pragma once

#include "HookSystem.h"
#include "TimerChecker.h"

class FrameLimiter
{
private:
	TimeChecker frameRateTimer;

public:
	FrameLimiter();
	void InitFrameLimiter();
	void OnGametimerUpdate();
	void ApplyQueuedFrameRateLimit();
	int Test() { return 1;  }
};
