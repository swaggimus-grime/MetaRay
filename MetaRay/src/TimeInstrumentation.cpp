#include "TimeInstrumentation.h"

#include <chrono>

using std::chrono::high_resolution_clock;

static high_resolution_clock s_Clock;
static high_resolution_clock::time_point s_Start;

void TimeInst::Start()
{
	s_Start = s_Clock.now();
}

float TimeInst::End()
{
	return (s_Clock.now() - s_Start).count();
}