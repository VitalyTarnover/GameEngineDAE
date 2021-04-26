#pragma once
#include "Components.h"

enum class MoveDirections 
{
	MoveUpRight,
	MoveDownRight,
	MoveUpLeft,
	MoveDownLeft
};


class MovementComponent final : public BaseComponent
{
public:
	MovementComponent() {};
	void Move(MoveDirections direction);
	void Update();
private:
	glm::vec3 m_CurrentPos;
	glm::vec3 m_DesiredPos;
	MoveDirections m_MoveDirection;
	bool m_Jumped;
	float m_JumpTime;
};

