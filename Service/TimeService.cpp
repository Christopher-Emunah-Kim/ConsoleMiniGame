#include "TimeService.h"

void TimeService::Init()
{
	m_prevTimePoint = steady_clock::now();
}

bool TimeService::CanUpdate()
{
	TimePoint current = high_resolution_clock::now();

	duration<float> delta = current - m_prevTimePoint;

	if (TICK_INTERVAL_MS * 0.001f > delta.count())
	{
		return false;
	}

	m_deltaTime = delta.count();
	m_prevTimePoint = current;

	return true;
}