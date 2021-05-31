#pragma once
#include "Structs.h"

class AnalogTriggerCommand abstract
{
public:
	AnalogTriggerCommand(int index) :m_ControllerIndex{ index } {};

	virtual ~AnalogTriggerCommand() = default;
	virtual void Execute() const = 0;
	virtual void Undo() = 0;
	void SetTriggerValue(float triggerValue) { m_TriggerValue = triggerValue; };
protected:
	float m_TriggerValue = 0.f;
	int m_ControllerIndex;
};