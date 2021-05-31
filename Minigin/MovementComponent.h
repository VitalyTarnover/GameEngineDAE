#pragma once
#include "Components.h"

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

	void KeyReleased(MoveDirections moveDir) { m_IsKeyPressed[(int)moveDir] = false; }

	void Update() override;

	void Jump();
	void ActivateJump();

	void FallToDeath();

	bool IsMoving() const { return m_IsMoving; }

	void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };

	bool GetIsFallingToDeathBehindMap() const 
	{ return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == AnimStates::MidAirLeftUp || m_Direction == AnimStates::MidAirRightUp); };

private:
	MoveDirections m_MoveDirection;
	bool m_IsMoving;
	bool m_FallingToDeath;
	glm::vec3 m_MoveDistance;
	float m_Speed;
	bool m_FirstHalfOfTheJump;
	AnimStates m_Direction;

	glm::vec3 m_JumpStartPos;
	int m_CurrentCubeIndex;

	bool m_IsKeyPressed[4];

};

