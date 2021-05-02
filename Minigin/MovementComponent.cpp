#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Time.h"


MovementComponent::MovementComponent()
	: m_Jumped{ false }
	, m_GoingUp{ false }
	, m_CurrentPos{}
	, m_DesiredPosLeft{}
	, m_DesiredPosRight{}
	, m_DesiredPosTop{}
	, m_DesiredPosBottom{}
	, m_JumpDirection{}
	, m_MoveDirection{}
	, m_JumpSpeed{ 25 }
	, m_IsKeyPressed{ false }
{
	const glm::vec3& cubeOffset = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->GetOffset();
	m_MoveDistance = cubeOffset;
}

void MovementComponent::Move(MoveDirections moveDir)
{
	m_CurrentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	m_DesiredPosLeft = m_CurrentPos.x;
	m_DesiredPosRight = m_CurrentPos.x;
	m_DesiredPosTop = m_CurrentPos.y;
	m_DesiredPosBottom = m_CurrentPos.y;

	m_DesiredPosLeft -= 80;
	m_DesiredPosRight += 80;
	m_DesiredPosTop -= 120;
	m_DesiredPosBottom += 120;
	
	
	m_MoveDirection = moveDir;
	m_IsKeyPressed[(int)moveDir] = true;


	if (m_IsKeyPressed[(int)MoveDirections::Up] && m_IsKeyPressed[(int)MoveDirections::Left])
	{
		m_JumpDirection.x = -1;
		m_JumpDirection.y = -1;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftUp);
		m_Jumped = true;
		m_GoingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirections::Up] && m_IsKeyPressed[(int)MoveDirections::Right])
	{
		m_JumpDirection.x = 1;
		m_JumpDirection.y = -1;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightUp);
		m_Jumped = true;
		m_GoingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirections::Down] && m_IsKeyPressed[(int)MoveDirections::Right])
	{
		m_JumpDirection.x = 1;
		m_JumpDirection.y = 1;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
		m_Jumped = true;
		m_GoingUp = true;
	}
	else if (m_IsKeyPressed[(int)MoveDirections::Down] && m_IsKeyPressed[(int)MoveDirections::Left])
	{
		m_JumpDirection.x = -1;
		m_JumpDirection.y = 1;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
		m_Jumped = true;
		m_GoingUp = true;
	}
}

void MovementComponent::KeyReleased(MoveDirections moveDir)
{
	m_IsKeyPressed[(int)moveDir] = false;
}

void MovementComponent::Update()
{
	if (m_Jumped)
	{
		m_pGameObject->GetComponent<TransformComponent>()->SetTransform(m_CurrentPos);

		if (m_GoingUp)
		{
			m_CurrentPos.y += (200 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.y;

			if (m_CurrentPos.y <= m_DesiredPosTop - 20)
			{
				m_GoingUp = false;
			}
		}
		else
		{
			m_CurrentPos.y -= (200 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.y;
		}
		m_CurrentPos.x += (110 * Time::GetInstance().GetDeltaTime()) * m_JumpDirection.x;

		if (m_CurrentPos.x <= m_DesiredPosLeft)
		{
			m_CurrentPos.x = m_DesiredPosLeft;
		}

		//land
		if (m_CurrentPos.x == m_DesiredPosLeft && m_CurrentPos.y >= m_DesiredPosTop && !m_GoingUp)
		{
			m_Jumped = false;
		}
	}

}
