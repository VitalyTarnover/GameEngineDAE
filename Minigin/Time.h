#pragma once
#include "Singleton.h"

class Time final : public dae::Singleton<Time>
{
public:
	float GetDeltaTime();
	void SetDeltaTime(float deltaTime);
private:
	Time() = default;
	float m_DeltaTime{};
	friend class dae::Singleton<Time>;
};