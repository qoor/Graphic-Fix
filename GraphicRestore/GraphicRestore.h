#pragma once

#include <Windows.h>

#include "ini.h"
#include "Debug.h"
#include "Memory.h"

#include <iostream>
#include <fstream>
#include <string>

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

	float aspectratio;
	float sniperCrosshairScale;
	float aircraftMaxHeight;
	float aircraftMaxVelocity;
	float aircraftMaxVelocity_Square;

	//HANDLE process;

public:
	GraphicRestore();
	~GraphicRestore();
	bool ApplyGraphicSettings();

	int GetSunflareEnable() { return sunflare; }
	int GetGrassEnable() { return grass; }
	int GetShadowsEnable() { return shadows; }
	int GetMotionblurEnable() { return motionblur; }
	int GetZonenamesEnable() { return zonenames; }
	int GetCarnamesEnable() { return carnames; }
	std::string& GetWidescreenfix() { return widescreenfix; }

	void SetSunflareEnable(int enable) { sunflare = enable; }
	void SetGrassEnable(int enable) { grass = enable; }
	void SetShadowsEnable(int enable) { shadows = enable; }
	void SetMotionblurEnable(int enable) { motionblur = enable; }
	void SetZonenamesEnable(int enable) { zonenames = enable; }
	void SetCarnamesEnable(int enable) { carnames = enable; }
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
};
