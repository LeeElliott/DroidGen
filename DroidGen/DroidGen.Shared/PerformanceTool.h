#pragma once
#include <chrono>

class PerformanceTool
{
public:
	inline void StartTimer() { timeStart = std::chrono::steady_clock::now(); }
	void EndTimer() { timeEnd = std::chrono::steady_clock::now(); }

	auto GetSeconds() 
	{ 
		auto countedTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
		return countedTime; 
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> timeStart;
	std::chrono::time_point<std::chrono::steady_clock> timeEnd;

};

