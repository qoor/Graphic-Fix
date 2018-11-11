#include "stdafx.h"

#include "FrameLimiter.h"
#include "GraphicRestore.h"

FrameLimiter* fl;
extern GraphicRestore* gr;

#define HOOKPOS_CTimer_Update               0x561B10
#define HOOKSIZE_CTimer_Update              6
#define HOOKPOS_CTaskSimpleSwim_ProcessSwimmingResistance   0x68A4EF

static const DWORD CONTINUE_CTimer_Update = 0x561B16;
DWORD RETURN_CTaskSimpleSwim_ProcessSwimmingResistance = 0x68A50E;

void HOOK_CTimer_Update();
void HOOK_CTaskSimpleSwim_ProcessSwimmingResistance();

FrameLimiter::FrameLimiter()
{
	fl = this;
}

void FrameLimiter::InitFrameLimiter()
{
	HookInstall(HOOKPOS_CTimer_Update, reinterpret_cast<DWORD>(HOOK_CTimer_Update), HOOKSIZE_CTimer_Update);
	HookInstall(HOOKPOS_CTaskSimpleSwim_ProcessSwimmingResistance, reinterpret_cast<DWORD>(HOOK_CTaskSimpleSwim_ProcessSwimmingResistance), 6);

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

const float kfTimeStepOriginal = 1.66f;
void _declspec(naked) HOOK_CTaskSimpleSwim_ProcessSwimmingResistance()
{
	_asm
	{
		fsub    st, st(1)

		fld     dword ptr[esp + 16]
		lea     eax, [esi + 44h]
		mov     ecx, eax
		fmul    st, st(1)

		fdiv    ds : 0xB7CB5C
		fmul    kfTimeStepOriginal

		fstp    dword ptr[esp + 28]

		fld     dword ptr[esp + 20]
		fmul    st, st(1)

		fdiv    ds : 0xB7CB5C
		fmul    kfTimeStepOriginal

		fstp    dword ptr[esp + 32]
		fmul    dword ptr[esp + 24]

		fdiv    ds : 0xB7CB5C
		fmul    kfTimeStepOriginal

		jmp     RETURN_CTaskSimpleSwim_ProcessSwimmingResistance
	}
}
