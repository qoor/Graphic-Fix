#include "stdafx.h"
#include "ini.h"

Ini::Ini(const string& file_name, ini::INI_OPEN_TYPE type) : type(type)
{
	if (type == ini::ini_read)
	{
		if (rfile.is_open() == true)
			rfile.close();

		rfile.open(file_name);
	}
	else
	{
		if (wfile.is_open() == true)
			wfile.close();

		wfile.open(file_name);
	}

}

bool Ini::IsOpened()
{
	if ((type == ini::ini_read && rfile.is_open()) || (type == ini::ini_write && wfile.is_open()))
		return true;

	return false;
}

bool Ini::GetKey(const string& line, string& key)
{
	int pos = line.find('=');

	if (pos != string::npos)
	{
		key = line.substr(0, pos);

		return true;
	}

	return false;
}

bool Ini::GetValue(const string& line, string& src)
{
	int pos = line.find('=');

	if (pos != string::npos)
	{
		src = line.substr(pos + 1, line.length() - 2);

		return true;
	}

	return false;
}

bool Ini::GetValue(const std::string &line, int& src)
{
	std::string value;

	if (GetValue(line, value))
	{
		src = std::stoi(value);

		return true;
	}

	return false;
}

bool Ini::GetValue(const std::string &line, double& src)
{
	std::string value;

	if (GetValue(line, value))
	{
		src = std::stof(value);

		return true;
	}

	return false;
}

bool Ini::Write(const string& key, const string& value)
{
	if (wfile.is_open() == false)
		return false;

	wfile << key << '=' << value << std::endl;

	return true;
}

Ini::~Ini()
{
	rfile.close();
	wfile.close();
}
