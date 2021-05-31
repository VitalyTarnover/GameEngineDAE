#pragma once
#include <iostream>
#include "Structs.h"



class MoveCommand final : public AnalogStickCommand
{
public:
	MoveCommand(int index) :AnalogStickCommand(index) {};

	void Execute() const override { std::cout << "Move in direction: " << int(m_StickTiltValue.x) / 255.f << ", " << int(m_StickTiltValue.y) / 255.f << '\n'; }
};

class LookCommand final : public AnalogStickCommand
{
public:
	LookCommand(int index) :AnalogStickCommand(index) {};

	void Execute() const override { std::cout << "Look in direction: " << int(m_StickTiltValue.x) / 255.f << ", " << int(m_StickTiltValue.y) / 255.f << '\n'; }
};
