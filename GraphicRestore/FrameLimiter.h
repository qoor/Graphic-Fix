#pragma once

#include "HookSystem.h"
#include "TimerChecker.h"

class FrameLimiter
{
private:
	TimeChecker frameRateTimer;
	bool queuedFrameRateValid;
	bool doneFrameRateLimit;
	bool frameLimitEnabled;

public:
	FrameLimiter();
	void InitFrameLimiter();
	void OnGametimerUpdate();
	void EnsureFrameRateLimitApplied();
	void ApplyFrameRateLimit();
	void ApplyQueuedFrameRateLimit();
	int Test() { return 1;  }
};

extern FrameLimiter* fl;
