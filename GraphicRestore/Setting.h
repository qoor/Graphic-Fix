#pragma once

#include "Ini.h"
#include "Debug.h"
#include "GraphicRestore.h"

#include <iostream>

class Setting
{
public:
	Setting(GraphicRestore& orderobj, const std::string& file_name);
};
