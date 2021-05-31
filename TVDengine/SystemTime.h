#pragma once
#include "Singleton.h"

class SystemTime final : public dae::Singleton<SystemTime>
{
public:
	float GetDeltaTime();
	void SetDeltaTime(float deltaTime);
private:
	SystemTime() = default;
	float m_DeltaTime{};
	friend class dae::Singleton<SystemTime>;
};