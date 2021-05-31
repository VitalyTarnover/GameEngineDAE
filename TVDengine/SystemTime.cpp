#include "pch.h"
#include "SystemTime.h"

float SystemTime::GetDeltaTime()
{
	return m_DeltaTime;
};

void SystemTime::SetDeltaTime(float deltaTime)
{
	m_DeltaTime = deltaTime;
};