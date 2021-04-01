#pragma once
#include <iostream>
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

class AimCommand final : public AnalogTriggerCommand
{
public:
	AimCommand(int index) :AnalogTriggerCommand(index) {};

	void Execute() const override { std::cout << "Aim trigger value: " << int(m_TriggerValue) / 255.f << '\n'; }
	void Undo() override {};
};

class ShootCommand final : public AnalogTriggerCommand
{
public:
	ShootCommand(int index) :AnalogTriggerCommand(index) {};

	void Execute() const override { std::cout << "Shoot trigger value: " << int(m_TriggerValue) / 255.f << '\n'; }
	void Undo() override {};
};
