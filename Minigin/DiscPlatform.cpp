#include "MiniginPCH.h"
#include "DiscPlatform.h"
#include "SceneManager.h"
#include "LevelComponent.h"

DiscPlatform::DiscPlatform(const glm::vec3& finalPos)
	:m_FinalPos {finalPos}
{
	m_pGameObject = std::make_shared<GameObject>(("Disc"));
	
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
	if (m_IsMovingToTop)
	{
		m_pTransformComponent = m_pGameObject->GetComponent<TransformComponent>();

		glm::vec3 discPosition = m_pTransformComponent->GetTransform().GetPosition();

		if (abs(discPosition.x - m_FinalPos.x) > 5)
		{
			if (discPosition.x < m_FinalPos.x)//disc is on the left
			{
				m_pTransformComponent->SetPosition(glm::vec3{ discPosition.x + m_Speed, discPosition.y - m_Speed*2, 0 });
			}
			else if (discPosition.x > m_FinalPos.x)//disc is on the right
			{
				m_pTransformComponent->SetPosition(glm::vec3{ discPosition.x - m_Speed, discPosition.y - m_Speed*2, 0 });
			}
		}
		else
		{
			//m_pTransformComponent->SetPosition(glm::vec3{ m_FinalPos.x, discPosition.y, 0 });
			m_IsMovingToTop = false;
		}
	}
	
	

}

