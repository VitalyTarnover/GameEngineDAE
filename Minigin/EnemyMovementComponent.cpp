#include "MiniginPCH.h"
#include "EnemyMovementComponent.h"
#include "SceneManager.h"
#include "LevelComponent.h"

EnemyMovementComponent::EnemyMovementComponent(const std::shared_ptr<GameObject>& pPlayer, EnemyType enemyType)
	:m_pPlayer{ pPlayer }
	, m_CurrentJumpTime{ 0.0f }
	, m_MaxJumpTime{ 1.5f }
	, m_EnemyType{ enemyType }
{

	switch (m_EnemyType)
	{
	case EnemyMovementComponent::EnemyType::Coily:
		m_SourceHeightOffset = -8;
		break;
	case EnemyMovementComponent::EnemyType::WrongWay:
		m_SourceHeightOffset = 0;
		m_CurrentColumn = 0;
		m_CurrentRow = 6;
		m_CurrentCubeIndex = 27;
		m_FallToDeathTimer = 1.f;
		break;
	case EnemyMovementComponent::EnemyType::Ugg:
		m_SourceHeightOffset = 0;
		m_CurrentColumn = 6;
		m_CurrentRow = 6;
		m_CurrentCubeIndex = 6;
		m_FallToDeathTimer = 1.f;

		break;
	case EnemyMovementComponent::EnemyType::Sam:
		m_SourceHeightOffset = 0;
		m_CurrentColumn = 0;
		m_CurrentRow = 1;
		m_CurrentCubeIndex = 7;
		m_FallToDeathTimer = 1.f;

		break;
	case EnemyMovementComponent::EnemyType::Slick:
		m_SourceHeightOffset = 0;
		m_CurrentColumn = 1;
		m_CurrentRow = 1;
		m_CurrentCubeIndex = 1;
		m_FallToDeathTimer = 1.f;

		break;
	default:
		break;
	}


}

void EnemyMovementComponent::Update()
{
	if (m_EnemyType == EnemyType::WrongWay || m_EnemyType == EnemyType::Ugg)
	{
		if (m_FallingToDeath)
		{
			SidewaysFallToDeath();
		}
		else if (m_IsMoving)
		{
			SidewaysJump();
		}
	}
	else
	{
		BaseMovementComponent::Update();
	}

	m_CurrentJumpTime += SystemTime::GetInstance().GetDeltaTime();

	if (m_CurrentJumpTime >= m_MaxJumpTime)
	{
		if (m_EnemyType == EnemyType::Coily)
		{
			FollowPlayer();
		}
		else if (m_EnemyType == EnemyType::Sam || m_EnemyType == EnemyType::Slick)
		{
			Descend();
		}
		else if (m_EnemyType == EnemyType::WrongWay || m_EnemyType == EnemyType::Ugg)
		{
			SidewaysMovement();
		}

		m_CurrentJumpTime -= m_CurrentJumpTime;
	}
}

void EnemyMovementComponent::FollowPlayer()
{
	if (m_IsMoving)
	{
		return;
	}

	const int yOffset = 30;
	auto playerPos = m_pPlayer->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	auto pos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

	if (playerPos.x <= pos.x && playerPos.y - yOffset <= pos.y)
	{
		m_Direction = AnimStates::MidAirLeftUp;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftUp);
		ActivateJump();
	}
	else if (playerPos.x >= pos.x && playerPos.y - yOffset <= pos.y)
	{
		m_Direction = AnimStates::MidAirRightUp;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightUp);
		ActivateJump();
	}
	else if (playerPos.x <= pos.x && playerPos.y >= pos.y)
	{
		m_Direction = AnimStates::MidAirLeftDown;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
		ActivateJump();
	}
	else if (playerPos.x >= pos.x && playerPos.y >= pos.y)
	{
		m_Direction = AnimStates::MidAirRightDown;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
		ActivateJump();
	}
}

void EnemyMovementComponent::Descend()
{
	if (m_IsMoving)
	{
		return;
	}

	int randNr = rand() % 2;

	if (randNr == 0)
	{
		m_Direction = AnimStates::MidAirLeftDown;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
		ActivateJump();
	}
	else
	{
		m_Direction = AnimStates::MidAirRightDown;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
		ActivateJump();
	}
}

void EnemyMovementComponent::SidewaysMovement()
{
	if (m_IsMoving)
	{
		return;
	}
	int dir = rand() % 2;

	if (m_EnemyType == EnemyType::WrongWay)
	{
		if (dir == 0)
		{
			m_Direction = AnimStates::MidAirRightDown;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightDown);
			ActivateJump(true);
			++m_CurrentColumn;
		}
		else
		{
			m_Direction = AnimStates::MidAirRightUp;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirRightUp);
			ActivateJump(true);
			--m_CurrentRow;
		}
	}
	else if(m_EnemyType == EnemyType::Ugg)
	{
		if (dir == 0)
		{
			m_Direction = AnimStates::MidAirLeftDown;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftDown);
			ActivateJump(true);
			--m_CurrentColumn;
		}
		else
		{
			m_Direction = AnimStates::MidAirLeftUp;
			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::MidAirLeftUp);
			ActivateJump(true);
			--m_CurrentColumn;
			--m_CurrentRow;
		}
	}
}

void EnemyMovementComponent::SidewaysJump()
{
	float deltaTime = SystemTime::GetInstance().GetDeltaTime();
	
	const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
	glm::vec3 pos = transform->GetTransform().GetPosition();

	const float moveDistanceRatio = (m_MoveDistance.y / m_MoveDistance.x);
	float jumpHeight = m_MoveDistance.y / 2.0f;

	glm::vec2 speed = { m_Speed,m_Speed * moveDistanceRatio * (m_MoveDistance.y / jumpHeight) };

	if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirLeftDown)
	{
		speed.x *= 2;
	}

	if (m_Direction == AnimStates::MidAirRightDown || m_Direction == AnimStates::MidAirRightUp)
		pos.x += deltaTime * speed.x;
	else
		pos.x -= deltaTime * speed.x;

	if ((int)m_Direction >= (int)AnimStates::OnPlatformRightDown)
		jumpHeight = m_MoveDistance.y / 2.0f;
	else
		jumpHeight = m_MoveDistance.y * 1.5f;

	if (m_Direction != AnimStates::MidAirRightDown && m_Direction != AnimStates::MidAirLeftDown)
	{
		if (m_FirstHalfOfTheJump)
		{
			pos.y -= deltaTime * speed.y;

			if (abs(pos.y - m_JumpStartPos.y) > jumpHeight)
				m_FirstHalfOfTheJump = false;
		}
		else
		{
			pos.y += deltaTime * speed.y;
		}
	}

	if (abs(pos.x - m_JumpStartPos.x) > m_MoveDistance.x)
	{
		//landed on cube
		int NonJumpingSprite = (int)m_Direction - 1;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates(NonJumpingSprite));
		const auto& CurrentMap = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>();

		auto cube = CurrentMap->GetCube(m_CurrentCubeIndex);

		//offset fix
		auto cubePos = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		//auto srcRect = m_pGameObject->GetComponent<Texture2DComponent>()->GetSrcRect();

		if (m_EnemyType == EnemyType::WrongWay)
		{
			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec3(cubePos.x - 15, cubePos.y + 20 * 2 + m_SourceHeightOffset,0));
			//m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x - srcRect.w, cubePos.y + srcRect.h * 2 + m_SourceHeightOffset));
		}
		else if (m_EnemyType == EnemyType::Ugg)
		{
			m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec3(cubePos.x + 15 * 3.f, cubePos.y + 20 * 2 + m_SourceHeightOffset,0));
			//m_pGameObject->GetComponent<TransformComponent>()->SetPosition(glm::vec2(cubePos.x + srcRect.w * 2.5f, cubePos.y + srcRect.h * 2 + m_SourceHeightOffset));
		}

		m_IsMoving = false;
	}
	else
	{
		transform->SetPosition(pos);
	}
}

void EnemyMovementComponent::SidewaysFallToDeath()
{
	float deltaTime = SystemTime::GetInstance().GetDeltaTime();
	if(m_FallToDeathTimer > 0)
	{
		m_FallToDeathTimer -= deltaTime;

		const auto& transform = m_pGameObject->GetComponent<TransformComponent>();
		glm::vec3 pos = transform->GetTransform().GetPosition();

		if (m_EnemyType == EnemyType::WrongWay)
		{
			pos.x += deltaTime * m_Speed * 4;
		}
		else if (m_EnemyType == EnemyType::Ugg)
		{
			pos.x -= deltaTime * m_Speed * 4;
		}

		transform->SetPosition(pos);
	}
	else
	{
		m_IsInDeathZone = true;
		m_IsMoving = false;
		m_FallingToDeath = false;
	}
	
}