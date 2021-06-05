#include "MiniginPCH.h"
#include "BaseMovementComponent.h"
#include "SceneManager.h"
#include "LevelComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Time.h"

BaseMovementComponent::BaseMovementComponent()
	: m_Speed{ 500 }
	, m_IsMoving{ false }
	, m_CurrentCubeIndex{ 0 }
	, m_FallingToDeath{ false }
	, m_Direction{ AnimStates::OnPlatformLeftDown }
{
	const glm::vec2& cubeOffset = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->GetOffset();
	m_MoveDistance = cubeOffset;
}

void BaseMovementComponent::Update()
{

	if (m_FallingToDeath)
	{
		FallToDeath();
	}
	else if (m_IsMoving)
	{
		Jump();
	}
}

void BaseMovementComponent::ActivateJump(bool isSideWaysJump)
{

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	m_JumpStartPos = transform->GetTransform().GetPosition();
	m_IsMoving = true;
	m_FirstHalfOfTheJump = true;

	const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>();

	if (!CurrentMap->JumpToNextCube(m_CurrentCubeIndex, m_Direction, isSideWaysJump, m_CurrentColumn, m_CurrentRow))
	{
		// Player jumped off the map
		m_FallingToDeath = true;
	}

}

void BaseMovementComponent::Jump()
{
	float elapsedTime = SystemTime::GetInstance().GetDeltaTime();

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec2 pos = transform->GetTransform().GetPosition();

	const float moveDistRatio = (m_MoveDistance.y / m_MoveDistance.x);

	float jumpHeight = m_MoveDistance.y / 2.0f;

	const glm::vec2 speed = { m_Speed, m_Speed * moveDistRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirRightUp)
		pos.x += elapsedTime * speed.x;
	else pos.x -= elapsedTime * speed.x;

	if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirLeftDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else jumpHeight = m_MoveDistance.y * 1.5f;

	if (m_FirstHalfOfTheJump)
	{
		pos.y -= elapsedTime * speed.y;

		if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
			m_FirstHalfOfTheJump = false;
	}
	else pos.y += elapsedTime * speed.y;

	if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
	{
		//landing
		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>();

		switch (m_Direction)
		{
		case (AnimStates::MidAirLeftDown):
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftDown);
			break;
		case (AnimStates::MidAirLeftUp):
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftUp);
			break;
		case (AnimStates::MidAirRightDown):
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformRightDown);
			break;
		case (AnimStates::MidAirRightUp):
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformRightUp);
			break;
		}


		if (m_pGameObject->GetName() == "Q*Bert" || m_pGameObject->GetName() == "Q*Bert2")
			CurrentMap->GetCube(m_CurrentCubeIndex)->SwitchColor(m_pGameObject->GetName());

		else if (m_pGameObject->GetName() == "Sam" || m_pGameObject->GetName() == "Slick")
			CurrentMap->GetCube(m_CurrentCubeIndex)->ResetCubeState();

		




		m_IsMoving = false;

		auto cube = CurrentMap->GetCube(m_CurrentCubeIndex);
		//offset fix
		//m_pGameObject->GetComponent<TransformComponent>()->SetPosition(cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition());
		pos.x = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 8.f);
		pos.y = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 10.f);

		m_IsMoving = false;

	}

	transform->SetPosition(glm::vec3(pos.x, pos.y, 0));
}

void BaseMovementComponent::FallToDeath()
{
	float elapsedTime = SystemTime::GetInstance().GetDeltaTime();

	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();

	glm::vec3 pos = transform->GetTransform().GetPosition();

	const float moveDistRatio = (m_MoveDistance.y / m_MoveDistance.x);

	float jumpHeight = m_MoveDistance.y / 2.0f;

	const glm::vec2 speed = { m_Speed,m_Speed * moveDistRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirRightUp)
		pos.x += elapsedTime * speed.x;
	else
		pos.x -= elapsedTime * speed.x;

	if ((int)m_Direction >= (int)AnimStates::OnPlatformRightDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else
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

		if (m_pGameObject->GetName() == "Q*bert")
		{
			auto pPlayerActor = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
			pPlayerActor.get()->GetComponent<HealthComponent>()->Die();
		}
		
	}
	else transform->SetPosition(glm::vec3(pos.x, pos.y, 0));
}