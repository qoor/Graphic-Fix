#include "stdafx.h"

#include "FrameLimiter.h"

#define HOOKPOS_CTimer_Update               0x561B10
#define HOOKSIZE_CTimer_Update              6

static const DWORD CONTINUE_CTimer_Update = 0x561B16;

extern FrameLimiter* fl;

void _declspec(naked) HOOK_CTimer_Update()
{
	_asm
	{
		pushad
	}

	fl->OnGametimerUpdate();

	_asm
	{
		popad
		mov     ecx, dword ptr ds : [0B7CB28h]
		jmp     CONTINUE_CTimer_Update
	}
}

FrameLimiter::FrameLimiter()
{
	fl = this;

	HookInstall(HOOKPOS_CTimer_Update, reinterpret_cast<DWORD>(HOOK_CTimer_Update), HOOKSIZE_CTimer_Update);

	DEBUG << "CTimer_Update ÈÄÅ· µÊ." << std::endl;
}

void FrameLimiter::OnGametimerUpdate()
{
	ApplyQueuedFrameRateLimit();
}

void FrameLimiter::ApplyQueuedFrameRateLimit()
{
	const double dTargetTimeToUse = 1000.0 / gr->GetFrameLimit();

	while (true)
	{
		double spare = dTargetTimeToUse - frameRateTimer.Get();

		if (spare <= 0.0)
			break;
		else if (spare >= 2.0)
			Sleep(1);
	}

	frameRateTimer.Reset();
}
