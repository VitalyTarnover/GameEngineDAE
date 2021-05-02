#pragma once
#include "Components.h"

//enum class MoveDirections 
//{
//	MoveUpRight,
//	MoveDownRight,
//	MoveUpLeft,
//	MoveDownLeft
//};


enum class MoveDirections 
{
	Up,
	Down,
	Left,
	Right
};


class MovementComponent final : public BaseComponent
{
public:
	MovementComponent();
	
	void Move(MoveDirections direction);

	void KeyReleased(MoveDirections moveDir);

	void Update() override;
private:
	glm::vec3 m_CurrentPos;
	//glm::vec3 m_DesiredPos;
	float m_DesiredPosLeft;
	float m_DesiredPosRight;
	float m_DesiredPosTop;
	float m_DesiredPosBottom;

	glm::vec3 m_JumpDirection;
	MoveDirections m_MoveDirection;
	glm::vec3 m_MoveDistance;

	int m_JumpSpeed;
	bool m_Jumped;
	bool m_GoingUp;
	float m_JumpTime;

	bool m_IsKeyPressed[4];
};

