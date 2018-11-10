#include "stdafx.h"

#include "Setting.h"

Setting::Setting(const std::string& file_name)
{
	DEBUG << gr << std::endl;

	Ini* file = new Ini(file_name, ini::ini_read);

	if (file->IsOpened() == true)
	{
		string line;
		string key;
		string value;

		while (std::getline(file->rfile, line))
		{
			file->GetKey(line, key);

			if (key.compare("SunFlare") == 0) { file->GetValue(line, value); gr->SetSunflareEnable(std::stoi(value)); }
			else if (key.compare("GrassAndPlants") == 0) { file->GetValue(line, value); gr->SetGrassEnable(std::stoi(value)); }
			else if (key.compare("PlayerShadows") == 0) { file->GetValue(line, value); gr->SetShadowsEnable(std::stoi(value)); }
			else if (key.compare("MotionBlur") == 0) { file->GetValue(line, value); gr->SetMotionblurEnable(std::stoi(value)); }
			else if (key.compare("ZoneNames") == 0) { file->GetValue(line, value); gr->SetZonenamesEnable(std::stoi(value)); }
			else if (key.compare("CarNames") == 0) { file->GetValue(line, value); gr->SetCarnamesEnable(std::stoi(value)); }
			else if (key.compare("WidescreenFix") == 0) { file->GetValue(line, value); gr->SetWidescreenfix(value); }
			else if (key.compare("FrameLimit") == 0) { file->GetValue(line, value); gr->SetFrameLimit(std::stoul(value)); }
		}

		delete file;

		DEBUG << "GraphicRestore을 로드합니다." << std::endl;
		DEBUG << "SunFlare = " << gr->GetSunflareEnable() << std::endl;
		DEBUG << "GrassAndPlants = " << gr->GetGrassEnable() << std::endl;
		DEBUG << "PlayerShadows = " << gr->GetShadowsEnable() << std::endl;
		DEBUG << "MotionBlur = " << gr->GetMotionblurEnable() << std::endl;
		DEBUG << "ZoneNames = " << gr->GetZonenamesEnable() << std::endl;
		DEBUG << "CarNames = " << gr->GetCarnamesEnable() << std::endl;
		DEBUG << "WidescreenFix = " << gr->GetWidescreenfix() << std::endl;
		DEBUG << "FrameLimit = " << gr->GetFrameLimit() << std::endl;
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
			file->Write("FrameLimit", "60");
		}

		delete file;
	}
}
