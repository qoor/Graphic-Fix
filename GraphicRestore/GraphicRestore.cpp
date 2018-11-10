#include "stdafx.h"

#include "GraphicRestore.h"
#include "Setting.h"
#include "FrameLimiter.h"

using std::string;

GraphicRestore* gr;

GraphicRestore::GraphicRestore() : sunflare(1), grass(1), shadows(0), motionblur(0), zonenames(0), carnames(1), widescreenfix("4:3"), framelimit(60)
{
	DEBUG << "객체 포인터 저장." << std::endl;

	gr = this;

	DEBUG << "설정을 불러옵니다." << std::endl;

	Setting setting("GraphicSettings.ini");

	DEBUG << "설정을 모두 불러왔습니다." << std::endl;

	aircraftMaxHeight = 800.0f;
	aircraftMaxVelocity = 1.5f;
	aircraftMaxVelocity_Square = aircraftMaxVelocity * aircraftMaxVelocity;
}

/*bool GraphicRestore::FindGameProcess()
{
	DWORD dwID;
	HWND game;

	game = FindWindow(NULL, L"GTA: San Andreas");

	GetWindowThreadProcessId(game, &dwID);

	process = OpenProcess(PROCESS_ALL_ACCESS, 0, dwID);

	return (process != NULL);
}*/

bool GraphicRestore::ApplyGraphicSettings()
{
	DEBUG << "gta_sa.exe 프로세스를 찾았습니다." << std::endl;

	// Allow all screen aspect ratios
	MemPut<WORD>(0x00745BC9, 0x9090);

	// Allow all screen aspect ratios in multi-monitor dialog
	MemPut<WORD>(0x007459E1, 0x9090);

	if (sunflare)
		MemPut(0x0053C136, 470050280, 5);
	
	if (grass)
		MemPut<int>(0x0053C159, 168706792, 5);

	if (shadows)
	{
		MemPut<long long>(0x0053EA08, 3288551609LL, 5);
		MemPut<int>(0x0053EA0D, 478191336, 5);
	}

	if (motionblur)
		MemPut<long long>(0x00704E8A, 1099509666280LL, 5);

	if (zonenames)
	{
		MemPut<long long>(0x005720A5, 1099511527144LL, 5);
		MemPut<int>(0x00590099, 427475688, 5);
		MemPut<long long>(0x005952A6, 1099508450792LL, 5);
	}

	if (carnames)
		MemPut<long long>(0x0058FBE9, 1099506561768LL, 5);
	
	if (widescreenfix.compare("4:3") != 0)
	{
		MemPut<float>(0x00859524, (0.002232143f / (4.0f / 3.0f) * aspectratio));

		// Set the sniper crosshair scale (fix for #7659)
		sniperCrosshairScale = 210.0f * (4.0f / 3.0f) / aspectratio;

		// Set the camera crosshair scale (same display flaw as in #7659)
		MemPut<float>(0x00866C74, 192.0f * (4.0f / 3.0f) / aspectratio);
	}
	else
	{
		sniperCrosshairScale = 210.f;

		MemPut<float>(0x00859524, 0.002232143f);
	}

	MemPut<DWORD>(0x0058E7D4 + 2, (DWORD)&sniperCrosshairScale);
	MemPut<DWORD>(0x0058E7EA + 2, (DWORD)&sniperCrosshairScale);
	MemPut<DWORD>(0x0053E3ED + 2, (DWORD)&sniperCrosshairScale);
	MemPut<DWORD>(0x0053E41A + 2, (DWORD)&sniperCrosshairScale);
	MemPut<DWORD>(0x0053E488 + 2, (DWORD)&sniperCrosshairScale);
	MemPut<DWORD>(0x0053E4BF + 2, (DWORD)&sniperCrosshairScale);

	// FPS Delay (12ms?) remover
	*(reinterpret_cast<BYTE*>(0x00BAB318)) = 0; *(reinterpret_cast<BYTE*>(0x0053E94C)) = 0;

	// Anti-crash
	MemCopy(reinterpret_cast<void*>(0x82C5CC), "\xC9\xC3", 2);

	// Increase intensity of vehicle tail light corona
	MemPut<BYTE>(0x006E1A22, 0xF0);

	/*// Fix killing ped during car jacking
	MemPut<BYTE>(0x63F576, 0xEB);*/

	// Aircraft Max Height checks are at 0x6D2614 and 0x6D2625 edit the check to use our own float.
	MemPut(0x006D2614, &aircraftMaxHeight);
	MemPut(0x006D2625, &aircraftMaxHeight);

	// Aircraft Max Velocity
	MemPut(0x006DADDF, &aircraftMaxVelocity_Square);
	MemPut(0x006DADEF, &aircraftMaxVelocity);
	MemPut(0x006DADF8, &aircraftMaxVelocity);
	MemPut(0x006DAE01, &aircraftMaxVelocity);

	// Disable setting the occupied's vehicles health to 75.0f when a burning ped enters it
	MemSet(reinterpret_cast<void*>(0x0053A651), 0x90, 0xA);

	// Allow to switch weapons while glued
	MemSetFast(reinterpret_cast<void*>(0x0060D861), 0x90, 14);

	// Clip camera also outside the world bounds.
	MemSet(reinterpret_cast<void*>(0x0041AD12), 0x90, 2);
	MemSet(reinterpret_cast<void*>(0x0041AD5E), 0x90, 2);
	MemSet(reinterpret_cast<void*>(0x0041ADA7), 0x90, 2);
	MemSet(reinterpret_cast<void*>(0x0041ADF3), 0x90, 2);

	// Disable re-initialization of DirectInput mouse device by the game
	MemPut<BYTE>(0x00576CCC, 0xEB);
	MemPut<BYTE>(0x00576EBA, 0xEB);
	MemPut<BYTE>(0x00576F8A, 0xEB);

	// Disable setting players on fire when they're riding burning bmx's
	MemPut<BYTE>(0x0053A982, 0xEB);

	// Do not fixate camera behind spectated player if local player is dead
	MemPut<BYTE>(0x0052A2BB, 0);
	MemPut<BYTE>(0x0052A4F8, 0);

	// Disable idle cam
	// MemPut<BYTE>(0x522C80, 0xC3);

	// Avoid GTA setting vehicle first color to white after changing the paintjob
	MemSet(reinterpret_cast<void*>(0x6D65C5), 0x90, 11);

	// Disable GTA vehicle detachment at rotation awkwardness
	MemPut<BYTE>(0x00547441, 0xE9);
	MemPut<BYTE>(0x00547442, 0xFA);
	MemPut<BYTE>(0x00547443, 0x02);
	MemPut<BYTE>(0x00547444, 0x00);
	MemPut<BYTE>(0x00547445, 0x00);

	// Fix for sliding over objects and vehicles (ice floor)
	MemPut<BYTE>(0x5E1E72, 0xE9);
	MemPut<BYTE>(0x5E1E73, 0xB9);
	MemPut<BYTE>(0x5E1E74, 0x00);
	MemPut<BYTE>(0x5E1E77, 0x90);

	// Fix vehicle back lights both using light state 3 (SA bug)
	MemPut<BYTE>(0x006E1D4F, 2);

	// Let us sprint everywhere (always return 0 from CSurfaceData::isSprint)
	MemPut<DWORD>(0x0055E870, 0xC2C03366);
	MemPut<WORD>(0x0055E874, 0x0004);

	// HACK to allow boats to be rotated
	MemPut<BYTE>(0x006F2089, 0x58);
	MemSet(reinterpret_cast<void*>(0x006F208A), 0x90, 4);

	// PREVENT CJ smoking and drinking like an addict
	MemPut<BYTE>(0x005FBA26, 0xEB);

	// ALLOW picking up of all vehicles (GTA doesn't allow picking up of 'locked' script created vehicles)
	MemPut<BYTE>(0x006A436C, 0x90);
	MemPut<BYTE>(0x006A436D, 0x90);

	// Allow water cannon to hit objects and players visually
	MemSet(reinterpret_cast<void*>(0x0072925D), 0x1, 1); // objects
	MemSet(reinterpret_cast<void*>(0x00729263), 0x1, 1); // players

	DEBUG << "성공적으로 적용했습니다." << std::endl;

	return true;
}

GraphicRestore::~GraphicRestore()
{
	Debug::Finalize();
}
