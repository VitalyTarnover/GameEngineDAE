#include "MiniginPCH.h"
#include "DiscPlatform.h"
#include "SceneManager.h"
#include "LevelComponent.h"
#include <cmath>

DiscPlatform::DiscPlatform(const glm::vec3& finalPos)
	:m_FinalPos {finalPos}
{

	m_pGameObject = std::make_shared<GameObject>(("Disc"));

	m_FinalPos.y -= 25 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	m_FinalPos.x += 5 * dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale();
	//auto currentLevelComponent = dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>();
	//auto finalPos = currentLevelComponent->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	
	
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
		if (int(animComp->GetCurrentAnimState()) < int(animComp->GetCoumnsNr()) - 1)
			animComp->SetAnimState( (AnimStates(int(animComp->GetCurrentAnimState()) + 1)) );
		else animComp->SetAnimState(AnimStates::DiscState1);

		m_AnimTimer = m_AnimTime;
	}

}

void DiscPlatform::MoveToTheTop()
{
	if (m_Direction == glm::vec3{ 0,0,0 })
	{
		auto dist = m_FinalPos - m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		auto length = sqrt((dist.x * dist.x) + (dist.y * dist.y));

		m_Direction.x = dist.x/length;
		m_Direction.y = dist.y/length;
	}

	if (m_IsMovingToTop)
	{
		m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec3 discPosition = m_pTransformComponent->GetTransform().GetPosition();

		
		if (abs(discPosition.x - m_FinalPos.x) > 5)
		{
			//float newXPos = std::lerp(m_FinalPos.x, discPosition.x, m_MoveFactor);
			//float newYPos = std::lerp(m_FinalPos.y, discPosition.y, m_MoveFactor);
			//m_pTransformComponent->SetPosition(glm::vec3{ newXPos, newYPos, 0 });
			//m_MoveFactor += SystemTime::GetInstance().GetDeltaTime() * m_Speed;

			auto newPos = discPosition + m_Direction * m_Speed;

			m_pTransformComponent->SetPosition(newPos);

		}
		else
		{
			m_pTransformComponent->SetPosition(glm::vec3{ m_FinalPos.x, m_FinalPos.y, 0 });
			m_IsMovingToTop = false;
			//mby kill here
			//dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->DeleteDisc(this);
		}
	}
}
