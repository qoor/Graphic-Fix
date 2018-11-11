/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        multiplayer_sa/CMultiplayerSA_CrashFixHacks.cpp
 *  PORPOISE:    Home for the poke'n'hope crash fixes
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "stdafx.h"

#include "GraphicRestore.h"

 //
 // Support for crash stats
 //
void OnCrashAverted(uint uiId);
void OnEnterCrashZone();

void OnEnterCrashZone()
{
	DEBUG << "Entered crash zone." << std::endl;
}

void OnCrashAverted(uint uiId)
{
	DEBUG << "Crash averted. uiID = " << uiId << std::endl;
}

void _declspec(naked) CrashAverted()
{
	_asm
	{
		pushfd
		pushad
		push[esp + 4 + 32 + 4 * 1]
		call    OnCrashAverted
		add     esp, 4
		popad
		popfd
		retn    4
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc1                              0x5D9A6E
#define HOOKSIZE_CrashFix_Misc1                             6
DWORD RETURN_CrashFix_Misc1 = 0x5D9A74;
void _declspec(naked) HOOK_CrashFix_Misc1()
{
	_asm
	{
		mov     eax, dword ptr[esp + 18h]
		test    eax, eax
		je      cont

		mov     eax, dword ptr ds : [008D12CCh]
		mov     ecx, dword ptr[eax + esi]     // If [eax+esi] is 0, it causes a crash
		test    ecx, ecx
		jne     cont
		push    1
		call    CrashAverted
		xor    ecx, ecx
		cont :
		jmp     RETURN_CrashFix_Misc1
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc2                              0x6B18B0
#define HOOKSIZE_CrashFix_Misc2                             9
DWORD RETURN_CrashFix_Misc2 = 0x6B18B9;
DWORD RETURN_CrashFix_Misc2B = 0x6B3775;
void _declspec(naked) HOOK_CrashFix_Misc2()
{
	_asm
	{
		test    eax, eax
		je      cont        // Skip much code if eax is zero (vehicle has no colmodel)

		mov     eax, dword ptr[eax + 2Ch]

		test    eax, eax
		je      cont        // Skip much code if eax is zero (colmodel has no coldata)

		mov     ebx, dword ptr[eax + 10h]

		test    ebx, ebx
		je      cont        // Skip much code if ebx is zero (coldata has no suspension lines)

		mov     cl, byte ptr[esi + 429h]
		jmp     RETURN_CrashFix_Misc2
		cont :
		push    2
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc2B
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc4                              0x4F02D2
#define HOOKSIZE_CrashFix_Misc4                             5
DWORD RETURN_CrashFix_Misc4 = 0x4F02D7;
DWORD RETURN_CrashFix_Misc4B = 0x4F0B07;
void _declspec(naked) HOOK_CrashFix_Misc4()
{
	_asm
	{
		test    ecx, ecx
		je      cont        // Skip much code if ecx is zero (avoid divide by zero in soundmanager::service)

		cdq
		idiv    ecx
		add     edx, ebp
		jmp     RETURN_CrashFix_Misc4
		cont :
		push    4
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc4B
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc5                              0x5DF949
#define HOOKSIZE_CrashFix_Misc5                             7
DWORD RETURN_CrashFix_Misc5 = 0x5DF950;
DWORD RETURN_CrashFix_Misc5B = 0x5DFCC4;
void _declspec(naked) HOOK_CrashFix_Misc5()
{
	_asm
	{
		mov     edi, dword ptr[ecx * 4 + 0A9B0C8h]
		mov     edi, dword ptr[edi + 5Ch]
		test    edi, edi
		je      cont        // Skip much code if edi is zero (ped has no model)

		mov     edi, dword ptr[ecx * 4 + 0A9B0C8h]
		jmp     RETURN_CrashFix_Misc5
		cont :
		push    5
			call    CrashAverted
			pop edi
			jmp     RETURN_CrashFix_Misc5B
	}
}

////////////////////////////////////////////////////////////////////////
// #5465 2/2
#define HOOKPOS_CrashFix_Misc6                              0x4D1750
#define HOOKSIZE_CrashFix_Misc6                             5
DWORD RETURN_CrashFix_Misc6 = 0x4D1755;
DWORD RETURN_CrashFix_Misc6B = 0x4D1A44;
void _declspec(naked) HOOK_CrashFix_Misc6()
{
	_asm
	{
		test    ecx, ecx
		je      cont        // Skip much code if ecx is zero (ped has no anim something)

		mov     eax, dword ptr[ecx + 10h]
		test    eax, eax
		jmp     RETURN_CrashFix_Misc6
		cont :
		push    6
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc6B
	}
}

////////////////////////////////////////////////////////////////////////
// #5466
#define HOOKPOS_CrashFix_Misc7                              0x417BF8
#define HOOKSIZE_CrashFix_Misc7                             5
DWORD RETURN_CrashFix_Misc7 = 0x417BFD;
DWORD RETURN_CrashFix_Misc7B = 0x417BFF;
void _declspec(naked) HOOK_CrashFix_Misc7()
{
	_asm
	{
		test    ecx, ecx
		je      cont        // Skip much code if ecx is zero (no colmodel)

		mov     esi, dword ptr[ecx + 2Ch]
		test    esi, esi
		jmp     RETURN_CrashFix_Misc7
		cont :
		push    7
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc7B
	}
}

////////////////////////////////////////////////////////////////////////
// #5468  1/3
#define HOOKPOS_CrashFix_Misc8                              0x73485D
#define HOOKSIZE_CrashFix_Misc8                             5
DWORD RETURN_CrashFix_Misc8 = 0x734862;
DWORD RETURN_CrashFix_Misc8B = 0x734871;
void _declspec(naked) HOOK_CrashFix_Misc8()
{
	_asm
	{
		test    ecx, ecx
		je      cont        // Skip much code if ecx is zero (no 2d effect plugin)

		mov     ecx, dword ptr[edx + ecx]
		test    ecx, ecx
		jmp     RETURN_CrashFix_Misc8
		cont :
		push    8
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc8B
	}
}

////////////////////////////////////////////////////////////////////////
// #5468  2/3
#define HOOKPOS_CrashFix_Misc9                              0x738B64
#define HOOKSIZE_CrashFix_Misc9                             6
DWORD RETURN_CrashFix_Misc9 = 0x738B6A;
DWORD RETURN_CrashFix_Misc9B = 0x73983A;
void _declspec(naked) HOOK_CrashFix_Misc9()
{
	_asm
	{
		test    esi, esi
		je      cont        // Skip much code if esi is zero (invalid projectile)

		mov     eax, dword ptr[esi + 40h]
		test    ah, 1
		jmp     RETURN_CrashFix_Misc9
		cont :
		push    9
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc9B
	}
}

////////////////////////////////////////////////////////////////////////
// #5468  3/3
#define HOOKPOS_CrashFix_Misc10                             0x5334FE
#define HOOKSIZE_CrashFix_Misc10                            6
DWORD RETURN_CrashFix_Misc10 = 0x533504;
DWORD RETURN_CrashFix_Misc10B = 0x533539;
void _declspec(naked) HOOK_CrashFix_Misc10()
{
	_asm
	{
		cmp     ecx, 0x80
		jb      cont  // Skip much code if ecx is small (invalid vector pointer)

		mov     edx, dword ptr[ecx]
		mov     dword ptr[esp], edx
		jmp     RETURN_CrashFix_Misc10
		cont :
		push    10
			call    CrashAverted
			mov     ecx, dword ptr[esp + 1Ch]
			mov     dword ptr[ecx], 0
			mov     dword ptr[ecx + 4], 0
			mov     dword ptr[ecx + 8], 0
			jmp     RETURN_CrashFix_Misc10B
	}
}

////////////////////////////////////////////////////////////////////////
// #5576
#define HOOKPOS_CrashFix_Misc11                             0x4D2C62
#define HOOKSIZE_CrashFix_Misc11                            5
DWORD RETURN_CrashFix_Misc11 = 0x4D2C67;
DWORD RETURN_CrashFix_Misc11B = 0x4D2E03;
void _declspec(naked) HOOK_CrashFix_Misc11()
{
	_asm
	{
		test    ecx, ecx
		je      cont  // Skip much code if ecx is zero (invalid anim somthing)

		mov     eax, dword ptr[ecx + 10h]
		test    eax, eax
		jmp     RETURN_CrashFix_Misc11
		cont :
		push    11
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc11B
	}
}

////////////////////////////////////////////////////////////////////////
// #5530
#define HOOKPOS_CrashFix_Misc12                             0x4D41C5
#define HOOKSIZE_CrashFix_Misc12                            5
DWORD RETURN_CrashFix_Misc12 = 0x4D41CA;
DWORD RETURN_CrashFix_Misc12B = 0x4D4222;
void _declspec(naked) HOOK_CrashFix_Misc12()
{
	_asm
	{
		test    edi, edi
		je      cont  // Skip much code if edi is zero (invalid anim somthing)

		mov     al, byte ptr[edi + 0Bh]
		test    al, al
		jmp     RETURN_CrashFix_Misc12
		cont :
		push    12
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc12B
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc13                             0x4D464E
#define HOOKSIZE_CrashFix_Misc13                            6
DWORD RETURN_CrashFix_Misc13 = 0x4D4654;
DWORD RETURN_CrashFix_Misc13B = 0x4D4764;
void _declspec(naked) HOOK_CrashFix_Misc13()
{
	_asm
	{
		cmp     eax, 0x2480
		jb      cont  // Skip much code if eax is less than 0x480 (invalid anim)

		mov     al, byte ptr[eax + 0Ah]
		shr     al, 5
		jmp     RETURN_CrashFix_Misc13
		cont :
		push    13
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc13B
	}
}

////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CrashFix_Misc14                             0x4DD4B5
#define HOOKSIZE_CrashFix_Misc14                            6
DWORD RETURN_CrashFix_Misc14 = 0x4DD4BB;
void _declspec(naked) HOOK_CrashFix_Misc14()
{
	_asm
	{
		mov     eax, dword ptr ds : [0BD00F8h]
		cmp     eax, 0
		je      cont  // Skip much code if eax is zero ( Audio event volumes table not initialized )

		sub     esp, 0D4h
		jmp     RETURN_CrashFix_Misc14
		cont :
		push    14
			call    CrashAverted
			add     esp, 12
			retn    12
	}
}

////////////////////////////////////////////////////////////////////////
void _cdecl DoWait(HANDLE hHandle)
{
	DWORD dwWait = 4000;
	DWORD dwResult = WaitForSingleObject(hHandle, dwWait);
	if (dwResult == WAIT_TIMEOUT)
	{
		DEBUG << "WaitForSingleObject timed out with %08x and %dms" << hHandle << dwWait << std::endl;
		// This thread lock bug in GTA will have to be fixed one day.
		// Until then, a 5 second freeze should be long enough for the loading thread to have finished it's job.
#if 0
		_wassert(_CRT_WIDE("\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            Press IGNORE  ('I' Key) \n\
            ")
			, _CRT_WIDE(__FILE__), __LINE__);
#endif
		dwResult = WaitForSingleObject(hHandle, 1000);
	}
}

// hook info
#define HOOKPOS_FreezeFix_Misc15_US                         0x156CDAE
#define HOOKSIZE_FreezeFix_Misc15_US                        6
#define HOOKPOS_FreezeFix_Misc15_EU                         0x156CDEE
#define HOOKSIZE_FreezeFix_Misc15_EU                        6
DWORD RETURN_FreezeFix_Misc15_US = 0x156CDB4;
DWORD RETURN_FreezeFix_Misc15_EU = 0x156CDF4;
DWORD RETURN_FreezeFix_Misc15_BOTH = 0;
void _declspec(naked) HOOK_FreezeFix_Misc15()
{
	_asm
	{
		pop eax
		pop edx
		pushad

		push eax
		call DoWait
		add esp, 4

		popad
		jmp     RETURN_FreezeFix_Misc15_BOTH
	}
}

////////////////////////////////////////////////////////////////////////
// Handle RpAnimBlendClumpGetFirstAssociation returning NULL
#define HOOKPOS_CrashFix_Misc16                             0x5E5815
#define HOOKSIZE_CrashFix_Misc16                            6
DWORD RETURN_CrashFix_Misc16 = 0x5E581B;
void _declspec(naked) HOOK_CrashFix_Misc16()
{
	_asm
	{
		cmp     eax, 0
		je      cont  // Skip much code if eax is zero ( RpAnimBlendClumpGetFirstAssociation returns NULL )

		// continue standard path
		movsx   ecx, word ptr[eax + 2Ch]
		xor edi, edi
		jmp     RETURN_CrashFix_Misc16

		cont :
		push    16
			call    CrashAverted
			add     esp, 96
			retn
	}
}

////////////////////////////////////////////////////////////////////////
// Handle RwFrameSetStaticPluginsSize having NULL member at 0x90
#define HOOKPOS_CrashFix_Misc17_US                          0x7F120E
#define HOOKSIZE_CrashFix_Misc17_US                         6
#define HOOKPOS_CrashFix_Misc17_EU                          0x7F124E
#define HOOKSIZE_CrashFix_Misc17_EU                         6
DWORD RETURN_CrashFix_Misc17_US = 0x7F1214;
DWORD RETURN_CrashFix_Misc17_EU = 0x7F1254;
DWORD RETURN_CrashFix_Misc17_BOTH = 0;
DWORD RETURN_CrashFix_Misc17B_US = 0x7F1236;
DWORD RETURN_CrashFix_Misc17B_EU = 0x7F1276;
DWORD RETURN_CrashFix_Misc17B_BOTH = 0;
void _declspec(naked) HOOK_CrashFix_Misc17()
{
	_asm
	{
		cmp     eax, 0
		je      cont  // Skip much code if eax is zero

		// continue standard path
		mov     eax, [eax + 90h]
		jmp     RETURN_CrashFix_Misc17_BOTH

		cont :
		push    17
			call    CrashAverted
			jmp     RETURN_CrashFix_Misc17B_BOTH
	}
}

////////////////////////////////////////////////////////////////////////
// Handle GetWheelPosition having wrong data
#define HOOKPOS_CrashFix_Misc18                             0x4C7DAD
#define HOOKSIZE_CrashFix_Misc18                            7
DWORD RETURN_CrashFix_Misc18 = 0x4C7DB4;
void _declspec(naked) HOOK_CrashFix_Misc18()
{
	_asm
	{
		cmp     ebp, 0
		je      cont  // Skip much code if ebp is zero

		// continue standard path
		mov         edx, dword ptr[ebp + 40h]
		mov         eax, dword ptr[esp + 10h]
		jmp     RETURN_CrashFix_Misc18

		cont :
		push    18
			call    CrashAverted
			mov         edx, 0
			mov         eax, dword ptr[esp + 10h]
			mov         dword ptr[eax], edx
			mov         dword ptr[eax + 4], edx
			pop         esi
			mov         dword ptr[eax + 8], edx
			pop         ebp
			ret         0Ch
	}
}

////////////////////////////////////////////////////////////////////////
// Handle RwFrameCloneHierarchy having wrong data
#define HOOKPOS_CrashFix_Misc19_US                          0x7F0BF7
#define HOOKSIZE_CrashFix_Misc19_US                         6
#define HOOKPOS_CrashFix_Misc19_EU                          0x7F0C37
#define HOOKSIZE_CrashFix_Misc19_EU                         6
DWORD RETURN_CrashFix_Misc19_US = 0x7F0BFD;
DWORD RETURN_CrashFix_Misc19_EU = 0x7F0C3D;
DWORD RETURN_CrashFix_Misc19_BOTH = 0;
DWORD RETURN_CrashFix_Misc19B_US = 0x7F0C20;
DWORD RETURN_CrashFix_Misc19B_EU = 0x7F0C60;
DWORD RETURN_CrashFix_Misc19B_BOTH = 0;
void _declspec(naked) HOOK_CrashFix_Misc19()
{
	_asm
	{
		cmp     esi, 0
		je      cont  // Skip much code if esi is zero

		// continue standard path
		mov     eax, [esi + 98h]
		jmp     RETURN_CrashFix_Misc19_BOTH

		cont :
		push    19
			call    CrashAverted
			mov     edx, dword ptr[ecx + 98h]
			test    edx, edx
			jmp     RETURN_CrashFix_Misc19B_BOTH
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CPlaceable::RemoveMatrix having wrong data
#define HOOKPOS_CrashFix_Misc20                             0x54F3B0
#define HOOKSIZE_CrashFix_Misc20                            6
DWORD RETURN_CrashFix_Misc20 = 0x54F3B6;
void _declspec(naked) HOOK_CrashFix_Misc20()
{
	_asm
	{
		cmp     ecx, 0
		je      cont        // Skip much code if ecx is zero

		// continue standard path
		sub     esp, 10h
		mov     eax, [ecx + 14h]
		jmp     RETURN_CrashFix_Misc20

		cont :
		push    20
			call    CrashAverted
			retn
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CAnimBlendAssociation::Init having wrong data
#define HOOKPOS_CrashFix_Misc22                             0x4CEF08
#define HOOKSIZE_CrashFix_Misc22                            6
DWORD RETURN_CrashFix_Misc22 = 0x4CEF25;
void _declspec(naked) HOOK_CrashFix_Misc22()
{
	_asm
	{
		mov         edx, dword ptr[edi + 0Ch]

		cmp     edx, 0x480
		jb      altcode  // Fill output with zeros if edx is low

		// do standard code
		lp1 :
		mov         edx, dword ptr[edi + 0Ch]
			mov         edx, dword ptr[edx + eax * 4]
			mov         ebx, dword ptr[esi + 10h]
			mov         dword ptr[ebx + ecx + 10h], edx
			mov         edx, dword ptr[esi + 10h]
			mov         dword ptr[edx + ecx + 14h], esi
			movsx       edx, word ptr[esi + 0Ch]
			inc         eax
			add         ecx, 18h
			cmp         eax, edx
			jl          lp1
			jmp     RETURN_CrashFix_Misc22

			// do alternate code
			altcode :
		push    22
			call    CrashAverted
			mov     edx, 0
			mov         ebx, dword ptr[esi + 10h]
			mov         dword ptr[ebx + ecx + 10h], edx
			mov         dword ptr[ebx + ecx + 14h], edx
			movsx       edx, word ptr[esi + 0Ch]
			inc         eax
			add         ecx, 18h
			cmp         eax, edx
			jl          altcode
			jmp     RETURN_CrashFix_Misc22
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CVehicleAnimGroup::ComputeAnimDoorOffsets having wrong door index
#define HOOKPOS_CrashFix_Misc23                             0x6E3D10
#define HOOKSIZE_CrashFix_Misc23                            7
DWORD RETURN_CrashFix_Misc23 = 0x6E3D17;
void _declspec(naked) HOOK_CrashFix_Misc23()
{
	_asm
	{
		// Ensure door index is reasonable
		mov     edx, [esp + 8]
		cmp     edx, 16
		jb      ok

		// zero if out of range
		mov     edx, 0
		mov[esp + 8], edx
		push    23
		call    CrashAverted

		ok :
		// continue standard path
		mov     edx, [esp + 8]
			lea     eax, [edx + edx * 2]
			jmp     RETURN_CrashFix_Misc23
	}
}

////////////////////////////////////////////////////////////////////////
// Handle _RwFrameForAllChildren being called with NULL
#define HOOKPOS_CrashFix_Misc24_US                          0x7F0DC8
#define HOOKSIZE_CrashFix_Misc24_US                         6
#define HOOKPOS_CrashFix_Misc24_EU                          0x7F0E08
#define HOOKSIZE_CrashFix_Misc24_EU                         6
DWORD RETURN_CrashFix_Misc24_US = 0x7F0DCE;
DWORD RETURN_CrashFix_Misc24_EU = 0x7F0E0E;
DWORD RETURN_CrashFix_Misc24_BOTH = 0;
void _declspec(naked) HOOK_CrashFix_Misc24()
{
	_asm
	{
		cmp     ebp, 0x480
		jb      cont  // Skip code if ebp is low

		// continue standard path
		mov     eax, [ebp + 98h]
		jmp     RETURN_CrashFix_Misc24_BOTH

		cont :
		push    24
			call    CrashAverted
			mov     ebp, 0
			mov     eax, 0
			jmp     RETURN_CrashFix_Misc24_BOTH
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CTaskSimpleCarOpenDoorFromOutside::FinishAnimCarOpenDoorFromOutsideCB having zero pointer to vehicle
#define HOOKPOS_CrashFix_Misc25                             0x646026
#define HOOKSIZE_CrashFix_Misc25                            5
DWORD RETURN_CrashFix_Misc25 = 0x64602B;
void _declspec(naked) HOOK_CrashFix_Misc25()
{
	_asm
	{
		// Check for zero pointer to vehicle
		mov     eax, [esi + 0x10]
		cmp     eax, 0
		jz      fix

		// Continue standard path
		lea     eax, [esp + 10h]
		push    eax
		jmp     RETURN_CrashFix_Misc25

		fix :
		push    25
			call    CrashAverted
			// Do special thing
			pop     esi
			pop     ecx
			retn
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CShotInfo::Update having invalid item
#define HOOKPOS_CrashFix_Misc26                             0x739FA0
#define HOOKSIZE_CrashFix_Misc26                            6
DWORD RETURN_CrashFix_Misc26 = 0x739FA6;
void _declspec(naked) HOOK_CrashFix_Misc26()
{
	_asm
	{
		// Check for incorrect pointer
		cmp     ebx, 130h
		jz      fix

		// Continue standard path
		mov     edi, dword ptr[ebx + ebp * 4]
		dec     ebp
		test    edi, edi
		jmp     RETURN_CrashFix_Misc26

		fix :
		push    26
			call    CrashAverted
			// Do special thing
			mov     edi, 0
			dec     ebp
			test    edi, edi
			jmp     RETURN_CrashFix_Misc26
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CTaskComplexDieInCar::ControlSubTask ped with no vehicle
#define HOOKPOS_CrashFix_Misc27                             0x6377FB
#define HOOKSIZE_CrashFix_Misc27                            7
DWORD RETURN_CrashFix_Misc27 = 0x637802;
void _declspec(naked) HOOK_CrashFix_Misc27()
{
	_asm
	{
		// Execute replaced code
		cmp     byte ptr[edi + 484h], 2
		je      cont

		// Check if veh pointer is zero
		mov     ecx, [edi + 58Ch]
		test    ecx, ecx
		jne     cont
		push    27
		call    CrashAverted

		cont :
		// Continue standard path
		jmp     RETURN_CrashFix_Misc27
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CObject::ProcessGarageDoorBehaviour object with no dummy
#define HOOKPOS_CrashFix_Misc28                             0x44A4FD
#define HOOKSIZE_CrashFix_Misc28                            6
DWORD RETURN_CrashFix_Misc28 = 0x44A503;
DWORD RETURN_CrashFix_Misc28B = 0x44A650;
void _declspec(naked) HOOK_CrashFix_Misc28()
{
	_asm
	{
		// Execute replaced code
		mov     eax, [esi + 170h]

		// Check if dummy pointer is zero
		test    eax, eax
		jne     cont

		push    28
		call    CrashAverted
		// Skip much code
		jmp     RETURN_CrashFix_Misc28B

		cont :
		// Continue standard path
		jmp     RETURN_CrashFix_Misc28
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CAEPCBankLoader::IsSoundBankLoaded with invalid argument
#define HOOKPOS_CrashFix_Misc29                             0x4E022C
#define HOOKSIZE_CrashFix_Misc29                            5
DWORD RETURN_CrashFix_Misc29 = 0x4E0231;
DWORD RETURN_CrashFix_Misc29B = 0x4E0227;
void _declspec(naked) HOOK_CrashFix_Misc29()
{
	_asm
	{
		// Execute replaced code
		movsx   eax, word ptr[esp + 8]

		// Check word being -1
		cmp     al, 0xffff
		jz      cont

		// Continue standard path
		jmp     RETURN_CrashFix_Misc29

		cont :
		push    29
			call    CrashAverted
			// Skip much code
			jmp     RETURN_CrashFix_Misc29B
	}
}

////////////////////////////////////////////////////////////////////////
// Handle CAnimBlendAssociation::SetFinishCallback with invalid ecx
#define HOOKPOS_CrashFix_Misc30                             0x4CEBE8
#define HOOKSIZE_CrashFix_Misc30                            7
#define HOOKCHECK_CrashFix_Misc30                           0xC7
DWORD RETURN_CrashFix_Misc30 = 0x4CEBEF;
DWORD RETURN_CrashFix_Misc30B = 0x4CEBF5;
void _declspec(naked) HOOK_CrashFix_Misc30()
{
	_asm
	{
		// Check for incorrect pointer
		cmp     ecx, 0
		jz      cont

		// Execute replaced code
		mov     dword ptr[ecx + 30h], 1
		// Continue standard path
		jmp     RETURN_CrashFix_Misc30

		cont :
		push    30
			call    CrashAverted
			// Skip much code
			jmp     RETURN_CrashFix_Misc30B
	}
}

struct CStreamingInfo
{
	DWORD gta_hash;
	WORD  chain_next;
	uchar flg;
	uchar archiveId;
	DWORD offsetInBlocks;
	DWORD sizeInBlocks;
	DWORD reqload;
};

CStreamingInfo* GetStreamingInfoFromModelId(uint id)
{
	CStreamingInfo* pItemInfo = (CStreamingInfo*)(0x8E4CC0);
	return pItemInfo + id;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Train crossings: Detach barrier from post (to be able to create objects 1373 and 1374 separately)
//
//////////////////////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CObject_Destructor_TrainCrossing_Check 0x59F7A8
#define HOOKPOS_CObject_ProcessTrainCrossingBehavior1 0x5A0C34
#define HOOKPOS_CObject_ProcessTrainCrossingBehavior2 0x5A0C54
#define RETURN_CObject_Destructor_TrainCrossing_Check 0x59F7AD
#define RETURN_CObject_Destructor_TrainCrossing_Invalid 0x59F811
#define RETURN_CObject_ProcessTrainCrossingBehavior1_Check 0x5A0C39
#define RETURN_CObject_ProcessTrainCrossingBehavior2_Check 0x5A0C59
#define RETURN_CObject_ProcessTrainCrossingBehavior_Invalid 0x5A0CBD

DWORD TrainCrossingFix_ReturnAddress, TrainCrossingFix_InvalidReturnAddress;
template <DWORD ReturnAddress, DWORD InvalidReturnAddress>
void _declspec(naked) HOOK_TrainCrossingBarrierCrashFix()
{
	// We cannot use template parameters directly in inline assembly; the following instructions don't modify registers
	TrainCrossingFix_ReturnAddress = ReturnAddress;
	TrainCrossingFix_InvalidReturnAddress = InvalidReturnAddress;

	_asm
	{
		test eax, eax // Check if pLinkedBarrierPost exists
		jz jmp_invalid // Skip the barrier stuff
		mov ecx, [eax + 14h] // Execute replaced code
		test ecx, ecx
		jmp TrainCrossingFix_ReturnAddress

		jmp_invalid :
		jmp TrainCrossingFix_InvalidReturnAddress
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// GTA doesn't reset the furniture object counter, so do it manually everytime before GTA furnishes an interior (Interior_c::Init)
//
//////////////////////////////////////////////////////////////////////////////////////////
#define HOOKPOS_ResetFurnitureObjectCounter 0x593BF0
#define HOOKSIZE_ResetFurnitureObjectCounter 6
DWORD RETURN_ResetFurnitureObjectCounter = 0x593BF6;
void _declspec(naked) HOOK_ResetFurnitureObjectCounter()
{
	*(int*)0xBB3A18 = 0;            // InteriorManager_c::ms_objectCounter

	_asm
	{
		// original instruction
		mov eax, fs:[0]
		jmp RETURN_ResetFurnitureObjectCounter
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// CVolumetricShadowMgr_Render
//
// Custom models can cause problems for volumetric shadows.
// Record when volumetric shadows are being rendered so we can disable them if a crash occurs.
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_CVolumetricShadowMgr_Render_Pre(void)
{
	OnEnterCrashZone();
}

void OnMY_CVolumetricShadowMgr_Render_Post(void)
{
	OnEnterCrashZone();
}

// Hook info
#define HOOKPOS_CVolumetricShadowMgr_Render                 0x7113B0
#define HOOKSIZE_CVolumetricShadowMgr_Render                8
#define HOOKCHECK_CVolumetricShadowMgr_Render               0x83
DWORD RETURN_CVolumetricShadowMgr_Render = 0x7113B8;
void _declspec(naked) HOOK_CVolumetricShadowMgr_Render()
{
	_asm
	{
		pushad
		call    OnMY_CVolumetricShadowMgr_Render_Pre
		popad

		call inner

		pushad
		call    OnMY_CVolumetricShadowMgr_Render_Post
		popad
		retn

		inner :
		// Replaced code
		sub     esp, 18h
			mov     ecx, 0A9AE00h
			jmp     RETURN_CVolumetricShadowMgr_Render
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// CVolumetricShadowMgr_Update
//
// Custom models can cause problems for volumetric shadows.
// Record when volumetric shadows are being updated so we can disable them if a crash occurs.
//
//////////////////////////////////////////////////////////////////////////////////////////
void OnMY_CVolumetricShadowMgr_Update_Pre(void)
{
	OnEnterCrashZone();
}

void OnMY_CVolumetricShadowMgr_Update_Post(void)
{
	OnEnterCrashZone();
}

// Hook info
#define HOOKPOS_CVolumetricShadowMgr_Update                 0x711D90
#define HOOKSIZE_CVolumetricShadowMgr_Update                5
#define HOOKCHECK_CVolumetricShadowMgr_Update               0xB9
DWORD RETURN_CVolumetricShadowMgr_Update = 0x711D95;
void _declspec(naked) HOOK_CVolumetricShadowMgr_Update()
{
	_asm
	{
		pushad
		call    OnMY_CVolumetricShadowMgr_Update_Pre
		popad

		push[esp + 4 * 1]
		call inner
		add     esp, 4 * 1

		pushad
		call    OnMY_CVolumetricShadowMgr_Update_Post
		popad
		retn

		inner :
		// Replaced code
		mov     ecx, 0A9AE00h
			jmp     RETURN_CVolumetricShadowMgr_Update
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Something called from CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask
//
// Accessing a temporally not existing vehicle
// (seems to happen when the driver is slower being thrown out than the jacker enters the vehicle)
//
//////////////////////////////////////////////////////////////////////////////////////////
#define HOOKPOS_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask   0x6485AC
#define HOOKSIZE_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask  6
DWORD RETURN_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask = 0x6485B2;
DWORD RETURN_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask_Invalid = 0x6485E1;
void  OnMY_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask()
{
	DEBUG << "CTaskComplexCarSlowBeDraggedOut race condition" << std::endl;
}

void _declspec(naked) HOOK_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask()
{
	_asm
	{
		test eax, eax
		jz invalid_vehicle

		mov ecx, [eax + 460h]
		jmp RETURN_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask

		invalid_vehicle :
		pushad
			call OnMY_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask
			popad
			jmp RETURN_CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask_Invalid
	}
}

////////////////////////////////////////////////////////////////////////
//
// RwMatrixMultiply
//
// Check for invalid matix pointer
//
////////////////////////////////////////////////////////////////////////
#define HOOKPOS_RwMatrixMultiply_US             0x7F18B0
#define HOOKSIZE_RwMatrixMultiply_US            6
#define HOOKCHECK_RwMatrixMultiply_US           0x8B
#define HOOKPOS_RwMatrixMultiply_EU             0x7F18F0
#define HOOKSIZE_RwMatrixMultiply_EU            6
#define HOOKCHECK_RwMatrixMultiply_EU           0x8B
DWORD RETURN_RwMatrixMultiply_US = 0x7F18B6;
DWORD RETURN_RwMatrixMultiply_EU = 0x7F18F6;
DWORD RETURN_RwMatrixMultiply_BOTH = 0;
void _declspec(naked) HOOK_RwMatrixMultiply()
{
	_asm
	{
		mov     eax, [esp + 0Ch]
		cmp     eax, 0x480
		jb      cont  // Skip code if eax is low

		mov     ecx, dword ptr ds : [0C979BCh]
		jmp     RETURN_RwMatrixMultiply_BOTH

		cont :
		push    31
			call    CrashAverted
			retn
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Setup hooks for CrashFixHacks
//
//////////////////////////////////////////////////////////////////////////////////////////
void GraphicRestore::InitCrashFix(void)
{
	// Anti-crash
	MemCopy(reinterpret_cast<void*>(0x82C5CC), "\xC9\xC3", 2);

	EZHookInstall(CrashFix_Misc1);
	EZHookInstall(CrashFix_Misc2);
	// EZHookInstall ( CrashFix_Misc3 );
	EZHookInstall(CrashFix_Misc4);
	EZHookInstall(CrashFix_Misc5);
	EZHookInstall(CrashFix_Misc6);
	EZHookInstall(CrashFix_Misc7);
	EZHookInstall(CrashFix_Misc8);
	EZHookInstall(CrashFix_Misc9);
	EZHookInstall(CrashFix_Misc10);
	EZHookInstall(CrashFix_Misc11);
	// EZHookInstall ( CrashFix_Misc12 );
	EZHookInstall(CrashFix_Misc13);
	EZHookInstall(CrashFix_Misc14);
	EZHookInstall(CrashFix_Misc16);
	// EZHookInstall ( CrashFix_Misc17 );
	EZHookInstall(CrashFix_Misc18);
	// EZHookInstall ( CrashFix_Misc19 );
	EZHookInstall(CrashFix_Misc20);
	EZHookInstall(CrashFix_Misc22);
	EZHookInstall(CrashFix_Misc23);
	EZHookInstall(CrashFix_Misc25);
	EZHookInstall(CrashFix_Misc26);
	EZHookInstall(CrashFix_Misc27);
	EZHookInstall(CrashFix_Misc28);
	EZHookInstall(CrashFix_Misc29);
	EZHookInstallChecked(CrashFix_Misc30);
	EZHookInstall(ResetFurnitureObjectCounter);
	// EZHookInstallChecked(CVolumetricShadowMgr_Render);
	// EZHookInstallChecked(CVolumetricShadowMgr_Update);
	EZHookInstall(CTaskComplexCarSlowBeDraggedOut_CreateFirstSubTask);

	// Install train crossing crashfix (the temporary variable is required for the template logic)
	void(*temp)() = HOOK_TrainCrossingBarrierCrashFix<RETURN_CObject_Destructor_TrainCrossing_Check, RETURN_CObject_Destructor_TrainCrossing_Invalid>;
	HookInstall(HOOKPOS_CObject_Destructor_TrainCrossing_Check, (DWORD)temp, 5);
	temp = HOOK_TrainCrossingBarrierCrashFix<RETURN_CObject_ProcessTrainCrossingBehavior1_Check, RETURN_CObject_ProcessTrainCrossingBehavior_Invalid>;
	HookInstall(HOOKPOS_CObject_ProcessTrainCrossingBehavior1, (DWORD)temp, 5);
	temp = HOOK_TrainCrossingBarrierCrashFix<RETURN_CObject_ProcessTrainCrossingBehavior2_Check, RETURN_CObject_ProcessTrainCrossingBehavior_Invalid>;
	HookInstall(HOOKPOS_CObject_ProcessTrainCrossingBehavior2, (DWORD)temp, 5);
}
