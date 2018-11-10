#pragma once

#include <chrono>

class TimeChecker
{
private:
	std::chrono::high_resolution_clock::time_point DoGetTickCount()
	{
		return std::chrono::high_resolution_clock::now();
	}

	std::chrono::high_resolution_clock::time_point resetTime;

public:
	TimeChecker()
	{
		Reset();
	}

	void Reset()
	{
		resetTime = DoGetTickCount();
	}

	double Get()
	{
		std::chrono::duration<double, std::micro> elapsedTime = DoGetTickCount() - resetTime;
		return elapsedTime.count() / 1000.0;
	}
};
