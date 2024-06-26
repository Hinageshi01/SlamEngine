#pragma once

#include "Core/Core.h"

#include <chrono>

namespace sl
{

class Timer final
{
public:
	Timer() = default;

	void Tick();

	// Returns in milliseconds.
	SL_FORCEINLINE float GetDeltatIme() const { return m_deltaTime; }

private:
	std::chrono::steady_clock::time_point m_lastTimePoint;
	float m_deltaTime = 0.0f;
};

} // namespace sl
