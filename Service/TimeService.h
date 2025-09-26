#pragma once
#include "../Core/Common.h"

using TimePoint = steady_clock::time_point;

static constexpr int32 FPS = 60;
static constexpr float TICK_INTERVAL_MS = 1000.0f / (float)FPS - 0.1f;

class TimeService
{
public:
	void Init();
	bool CanUpdate();

private:
	TimePoint m_prevTimePoint{};
};

