#include "MiniginPCH.h"
#include "DiscPlatform.h"

DiscPlatform::DiscPlatform()
{
	m_pGameObject = std::make_shared<GameObject>(("Disc"));
}

void DiscPlatform::Update()
{
	m_pGameObject->Update();
	SwitchColors();
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

