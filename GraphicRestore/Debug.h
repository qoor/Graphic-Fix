#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define DEBUG Debug::GetHandler()

class Debug {
private:
	Debug();
	~Debug();

public:
	static bool Initialize(const std::string& file_name);
	static std::ofstream& GetHandler();
	static bool Finalize();
	/*friend std::ostream& operator<<(Debug& obj, const std::string& value);*/
	bool Write(const std::string& value);
};
