#include "MiniginPCH.h"
#include "QbertMovementComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SystemTime.h"


QbertMovementComponent::QbertMovementComponent(bool autoStatrt, float speed)
	: m_IsKeyPressed{ false }
	, m_AutoStart {autoStatrt}
{
	m_SourceHeightOffset = 0;
	m_Speed = speed;
}

void QbertMovementComponent::Move(MoveDirections dir)
{
	if (!m_MovementLocked)
	{
		if (m_IsMoving)
		{
			return;
		}

		if (m_AutoStart)
		{
			int randDir = rand() % 2;

			if(randDir == 0)
			{
				m_Direction = AnimStates::MidAirLeftDown;
				m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
				ActivateJump();
				++m_CurrentRow;
			}
			else
			{
				m_Direction = AnimStates::MidAirRightDown;
				m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
				ActivateJump();
				++m_CurrentColumn;
				++m_CurrentRow;
			}
			if (m_CurrentRow >= 6) m_AutoStart = false;

			return;
		}
		m_MoveDirection = dir;
		m_IsKeyPressed[(int)dir] = true;

		if (m_IsKeyPressed[(int)MoveDirections::Up] 
			&& m_IsKeyPressed[(int)MoveDirections::Left])
		{
			m_Direction = AnimStates::MidAirLeftUp;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftUp);
			ActivateJump();
			--m_CurrentColumn;
			--m_CurrentRow;
		}
		else if (m_IsKeyPressed[(int)MoveDirections::Up] 
			&& m_IsKeyPressed[(int)MoveDirections::Right])
		{
			m_Direction = AnimStates::MidAirRightUp;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightUp);
			ActivateJump();
			--m_CurrentRow;
		}
		else if (m_IsKeyPressed[(int)MoveDirections::Down] 
			&& m_IsKeyPressed[(int)MoveDirections::Right])
		{
			m_Direction = AnimStates::MidAirRightDown;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
			ActivateJump();
			++m_CurrentColumn;
			++m_CurrentRow;
		}
		else if (m_IsKeyPressed[(int)MoveDirections::Down] 
			&& m_IsKeyPressed[(int)MoveDirections::Left])
		{
			m_Direction = AnimStates::MidAirLeftDown;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
			ActivateJump();
			++m_CurrentRow;
		}
	}
}


void QbertMovementComponent::ActivateJump(bool isSideWaysJump)
{


		const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

		m_JumpStartPos = transform->GetTransform().GetPosition();

		m_IsMoving = true;

		m_FirstHalfOfTheJump = true;

		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>();

		bool onMap = CurrentMap->JumpToNextCube(m_CurrentCubeIndex, m_Direction, isSideWaysJump, m_CurrentColumn, m_CurrentRow);

		if (m_pGameObject->GetName() != "Coily")
		{
			if (!onMap && !CurrentMap->GetCube(m_CurrentCubeIndex)->GetHasDiscNextToIt())
				m_FallingToDeath = true;
			else if (!onMap && CurrentMap->GetCube(m_CurrentCubeIndex)->GetHasDiscNextToIt())
				m_JumpingOnDisc = true;
		}
		else
		{
			if (!onMap) m_FallingToDeath = true;
		}

	
}

void QbertMovementComponent::JumpOnDisc()
{
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	if (!m_pDiscTransform)
	{
		if (m_IsOnDisc)
		{
		    dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->TeleportPlayersToSpawnPos();
			m_CurrentCubeIndex = 0;
			m_IsOnDisc = false;
			m_JumpingOnDisc = false;
			m_FallingToDeath = false;
		}
		else
		{
			float elapsedTime = SystemTime::GetInstance().GetDeltaTime();

			glm::vec3 pos = transform->GetTransform().GetPosition();

			const float moveDistRatio = (m_MoveDistance.y / m_MoveDistance.x);

			float jumpHeight = m_MoveDistance.y / 2.0f;

			const glm::vec2 speed = { m_Speed,m_Speed * moveDistRatio * (m_MoveDistance.y / jumpHeight) };

			if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirRightUp)
				pos.x += elapsedTime * speed.x;
			else
				pos.x -= elapsedTime * speed.x;

			jumpHeight = m_MoveDistance.y * 1.5f;

			if (m_FirstHalfOfTheJump)
			{
				pos.y -= elapsedTime * speed.y;

				if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
					m_FirstHalfOfTheJump = false;
			}
			else pos.y += elapsedTime * speed.y;

			if (pos.y > 720)
			{
				m_IsMoving = false;
				m_FallingToDeath = false;
			}
			else transform->SetPosition(glm::vec3(pos.x, pos.y, 0));
		}

	}
	else
	{
		m_IsOnDisc = true;
		glm::vec3 newPos = m_pDiscTransform->GetTransform().GetPosition();
		newPos.x += dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 1.f;
		newPos.y -= dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 13.f;
		transform->SetPosition(newPos);
	}

	
}



void QbertMovementComponent::Update()
{
	if (!m_MovementLocked)
	{
		if (m_FallingToDeath) FallToDeath();
		else if (m_JumpingOnDisc) JumpOnDisc();
		else if (m_IsMoving) Jump();
	}

	if (m_AutoStart)Move(MoveDirections::Right);


	UpdateMovementLockedWithTimer();

}
