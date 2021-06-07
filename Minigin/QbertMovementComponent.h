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
	QbertMovementComponent(bool autoStart = false, float speed = 100.f);
	~QbertMovementComponent()
	{
		m_pDiscTransform = nullptr;
		delete m_pDiscTransform;
	}

	void Move(MoveDirections direction);

	void KeyReleased(MoveDirections moveDir) { m_IsKeyPressed[(int)moveDir] = false; }

	void Update() override;

	void ActivateJump(bool isSideWaysJump = false);


	
	void JumpOnDisc();

	bool GetIsOnDisc() const { return m_IsOnDisc; }

	void LockMovementForSeconds(float seconds) { 
		m_MovementLockedWithTimer = true;
		m_MovementLockTimer = seconds; 
	};

	TransformComponent* GetDiscTransform() { return m_pDiscTransform; }
	void SetDiscTransform(TransformComponent* discTC) { m_pDiscTransform = discTC; }

private:
	MoveDirections m_MoveDirection;
	bool m_MovementLockedWithTimer = false;
	float m_MovementLockTimer = 0.f;
	bool m_JumpingOnDisc;
	bool m_IsOnDisc;
	TransformComponent* m_pDiscTransform;
	bool m_IsKeyPressed[4];

	bool m_AutoStart = false;

	void UpdateMovementLockedWithTimer()
	{
		if (m_MovementLockedWithTimer)
		{
			if (m_MovementLockTimer > 0)
			{
				m_MovementLocked = true;
				m_MovementLockTimer -= SystemTime::GetInstance().GetDeltaTime();
			}
			else
			{
				m_MovementLocked = false;
				m_MovementLockedWithTimer = false;
				m_MovementLockTimer = 0;
			}
		}

	}


};

