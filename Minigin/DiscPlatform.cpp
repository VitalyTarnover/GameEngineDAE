#include "MiniginPCH.h"
#include "DiscPlatform.h"
#include "SceneManager.h"
#include "LevelComponent.h"
#include "QbertMovementComponent.h"
#include "EnemyManager.h"
#include <cmath>



DiscPlatform::DiscPlatform(const glm::vec3& finalPos)
	:m_FinalPos {finalPos}
{
	m_pGameObject = std::make_shared<GameObject>(("Disc"));

	m_FinalPos.y -= 25 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	m_FinalPos.x += 5 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	
}

void DiscPlatform::Update()
{
	m_pGameObject->Update();
	SwitchColors();
	MoveToTheTop();
}

void DiscPlatform::Render() const
{
	m_pGameObject->Render();
}

void DiscPlatform::SwitchColors()
{
	if (m_AnimTimer > 0)
	{
		m_AnimTimer -= SystemTime::GetInstance().GetDeltaTime();
	}
	else
	{
		auto animComp = m_pGameObject->GetComponent<SpriteAnimComponent>();
		if (int(animComp->GetCurrentAnimState()) < int(animComp->GetColumnsNr()) - 1)
			animComp->SetAnimState( (AnimStates(int(animComp->GetCurrentAnimState()) + 1)) );
		else animComp->SetAnimState(AnimStates::DiscState1);

		m_AnimTimer = m_AnimTime;
	}

}

void DiscPlatform::MoveToTheTop()
{
	if (m_IsMovingToTop)
	{
		if (m_Direction == glm::vec3{ 0,0,0 })
		{
			auto dist = m_FinalPos - m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

			auto length = sqrt((dist.x * dist.x) + (dist.y * dist.y));

			m_Direction.x = dist.x / length;
			m_Direction.y = dist.y / length;
		}


		m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec3 discPosition = m_pTransformComponent->GetTransform().GetPosition();


		if (abs(discPosition.x - m_FinalPos.x) > 2)
		{

			auto newPos = discPosition + m_Direction * m_Speed * SystemTime::GetInstance().GetDeltaTime();

			m_pTransformComponent->SetPosition(newPos);

		}
		else
		{
			EnemyManager::GetInstance().DeleteAllEnemies();
			m_pTransformComponent->SetPosition(glm::vec3{ m_FinalPos.x, m_FinalPos.y, 0 });
			m_IsMovingToTop = false;
			m_IsUsed = true;
			dae::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(m_PlayerIndex)->GetComponent<QbertMovementComponent>()->SetDiscTransform(nullptr);
		}
	}
}

