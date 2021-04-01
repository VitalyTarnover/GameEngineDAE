#include "MiniginPCH.h"
#include "Time.h"

float Time::GetDeltaTime()
{
	return m_DeltaTime;
};

void Time::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
};