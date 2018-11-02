#pragma once

#include <fstream>
#include <iostream>
#include <string>

using std::string;

namespace ini {
	enum INI_OPEN_TYPE
	{
		ini_read,
		ini_write
	};
}

class Ini {
private:
	ini::INI_OPEN_TYPE type;

public:
	std::ifstream rfile;
	std::ofstream wfile;

	Ini(const std::string &file_name, ini::INI_OPEN_TYPE type);
	~Ini();
	bool IsOpened();
	bool GetKey(const string& line, string& key);
	bool GetValue(const std::string& line, std::string& src);
	bool GetValue(const std::string& line, int& src);
	bool GetValue(const std::string& line, double& src);
	bool Write(const std::string& key, const std::string& value);
};
