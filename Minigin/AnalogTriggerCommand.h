#pragma once
#include <iostream>
#include "Structs.h"
#include "BaseAnalogTriggerCommand.h"


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
