#include "stdafx.h"

#include "Debug.h"

namespace DebugHandler {
	std::ofstream file;
}

int debug_use = 0;

bool Debug::Initialize(const std::string& file_name)
{
	if (debug_use == 0 || DebugHandler::file.is_open())
		return false;

	DebugHandler::file.open(file_name);
	
	return true;
}

bool Debug::Finalize()
{
	if (debug_use == 0 || !DebugHandler::file.is_open())
		return false;

	DebugHandler::file.flush();

	return true;
}

Debug::~Debug()
{
	Finalize();
}

/*std::ostream& operator<<(Debug& obj, const std::string& value)
{
	obj.file << value;
}*/

bool Debug::Write(const std::string& value)
{
	if (debug_use == 0 || !DebugHandler::file.is_open())
		return false;

	DebugHandler::file << value;

	return true;
}

std::ofstream& Debug::GetHandler()
{
	return DebugHandler::file;
}
