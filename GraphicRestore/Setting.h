#pragma once

#include "Ini.h"
#include "Debug.h"
#include "GraphicRestore.h"

#include <iostream>

extern GraphicRestore* gr;

class Setting
{
public:
	Setting(const std::string& file_name);
};
