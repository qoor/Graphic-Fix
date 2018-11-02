#include "stdafx.h"

#include "Setting.h"

Setting::Setting(GraphicRestore& orderobj, const std::string& file_name)
{
	Ini* file = new Ini(file_name, ini::ini_read);

	if (file->IsOpened() == true)
	{
		string line;
		string key;
		string value;

		while (std::getline(file->rfile, line))
		{
			file->GetKey(line, key);

			if (key.compare("SunFlare") == 0) { file->GetValue(line, value); orderobj.SetSunflareEnable(std::stoi(value)); }
			else if (key.compare("GrassAndPlants") == 0) { file->GetValue(line, value); orderobj.SetGrassEnable(std::stoi(value)); }
			else if (key.compare("PlayerShadows") == 0) { file->GetValue(line, value); orderobj.SetShadowsEnable(std::stoi(value)); }
			else if (key.compare("MotionBlur") == 0) { file->GetValue(line, value); orderobj.SetMotionblurEnable(std::stoi(value)); }
			else if (key.compare("ZoneNames") == 0) { file->GetValue(line, value); orderobj.SetZonenamesEnable(std::stoi(value)); }
			else if (key.compare("CarNames") == 0) { file->GetValue(line, value); orderobj.SetCarnamesEnable(std::stoi(value)); }
			else if (key.compare("WidescreenFix") == 0) { file->GetValue(line, value); orderobj.SetWidescreenfix(value); }
		}

		delete file;

		DEBUG << "GraphicRestore을 로드합니다." << std::endl;
		DEBUG << "SunFlare = " << orderobj.GetSunflareEnable() << std::endl;
		DEBUG << "GrassAndPlants = " << orderobj.GetGrassEnable() << std::endl;
		DEBUG << "PlayerShadows = " << orderobj.GetShadowsEnable() << std::endl;
		DEBUG << "MotionBlur = " << orderobj.GetMotionblurEnable() << std::endl;
		DEBUG << "ZoneNames = " << orderobj.GetZonenamesEnable() << std::endl;
		DEBUG << "CarNames = " << orderobj.GetCarnamesEnable() << std::endl;
		DEBUG << "WidescreenFix = " << orderobj.GetWidescreenfix() << std::endl;
	}
	else
	{
		delete file;

		file = new Ini("GraphicSettings.ini", ini::ini_write);

		if (file->IsOpened())
		{
			file->Write("SunFlare", "1");
			file->Write("GrassAndPlants", "1");
			file->Write("PlayerShadows", "1");
			file->Write("MotionBlur", "0");
			file->Write("ZoneNames", "0");
			file->Write("CarNames", "0");
			file->Write("WidescreenFix", "4:3");
		}

		delete file;
	}
}
