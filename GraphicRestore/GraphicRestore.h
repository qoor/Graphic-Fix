#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include "ini.h"
#include "Debug.h"
#include "Memory.h"
#include "HookSystem.h"
#include "FrameLimiter.h"

//class FrameLimiter;

class GraphicRestore
{
private:
	int sunflare;
	int grass;
	int shadows;
	int motionblur;
	int zonenames;
	int carnames;
	std::string widescreenfix;
	unsigned int framelimit;

	float aspectratio;
	float sniperCrosshairScale;
	float aircraftMaxHeight;
	float aircraftMaxVelocity;
	float aircraftMaxVelocity_Square;

	FrameLimiter frameLimiter;

	//HANDLE process;

public:
	GraphicRestore();
	~GraphicRestore();
	bool ApplyGraphicSettings();

	inline int GetSunflareEnable() { return sunflare; }
	inline int GetGrassEnable() { return grass; }
	inline int GetShadowsEnable() { return shadows; }
	inline int GetMotionblurEnable() { return motionblur; }
	inline int GetZonenamesEnable() { return zonenames; }
	inline int GetCarnamesEnable() { return carnames; }
	std::string& GetWidescreenfix() { return widescreenfix; }
	inline unsigned int GetFrameLimit() { return framelimit; }

	inline void SetSunflareEnable(int enable) { sunflare = enable; }
	inline void SetGrassEnable(int enable) { grass = enable; }
	inline void SetShadowsEnable(int enable) { shadows = enable; }
	inline void SetMotionblurEnable(int enable) { motionblur = enable; }
	inline void SetZonenamesEnable(int enable) { zonenames = enable; }
	inline void SetCarnamesEnable(int enable) { carnames = enable; }
	void SetWidescreenfix(const std::string& value)
	{
		widescreenfix = value;

		if (widescreenfix.compare("4:3") == 0)
			aspectratio = 4 / 3.f;
		else if (widescreenfix.compare("16:9") == 0)
			aspectratio = 16 / 9.f;
		else if (widescreenfix.compare("16:10") == 0)
			aspectratio = 16 / 10.f;
	}
	inline void SetFrameLimit(unsigned int value) { framelimit = value; }

	void InitCrashFix();
};
