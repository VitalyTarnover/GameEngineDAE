#pragma once
#include "Components.h"
#include "BaseMovementComponent.h"

enum class MoveDirections
{
	Up,
	Down,
	Left,
	Right
};


class QbertMovementComponent final : public BaseMovementComponent
{
public:
	QbertMovementComponent();
	
	void Move(MoveDirections direction);

	void KeyReleased(MoveDirections moveDir) { m_IsKeyPressed[(int)moveDir] = false; }

	void Update() override;

	void ActivateJump(bool isSideWaysJump = false);
	//void Jump();

	//void FallToDeath();

	//bool IsMoving() const { return m_IsMoving; }

	//void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };

	//bool GetIsFallingToDeathBehindMap() const 
	//{ return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == AnimStates::MidAirLeftUp || m_Direction == AnimStates::MidAirRightUp); };

	
	void JumpOnDisc();

	TransformComponent* GetDiscTransform() { return m_pDiscTransform; }
	void SetDiscTransform(TransformComponent* discTC) { m_pDiscTransform = discTC; }
	//bool SetIsOnDisc(bool onDisc) { m_IsOnDisc = onDisc; }

private:
	MoveDirections m_MoveDirection;
	//bool m_IsMoving;
	//bool m_FallingToDeath;
	bool m_JumpingOnDisc;
	bool m_IsOnDisc;
	TransformComponent* m_pDiscTransform;
	bool m_IsKeyPressed[4];
	//glm::vec3 m_MoveDistance;
	//float m_Speed;
	//bool m_FirstHalfOfTheJump;
	//AnimStates m_Direction;


	//glm::vec3 m_JumpStartPos;
	//int m_CurrentCubeIndex;


};

