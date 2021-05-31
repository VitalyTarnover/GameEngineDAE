#pragma once
#include "Structs.h"

class AnalogStickCommand abstract
{
public:
	AnalogStickCommand(int index) :m_ControllerIndex{ index } {};

	virtual ~AnalogStickCommand() = default;
	virtual void Execute() const = 0;
	void SetAnalogStickValue(AnalogStickInput leftStickInput) { m_StickTiltValue.x = leftStickInput.x; m_StickTiltValue.y = leftStickInput.y; }
protected:
	AnalogStickInput m_StickTiltValue = {};
	int m_ControllerIndex;
};