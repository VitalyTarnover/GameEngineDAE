#include "MiniginPCH.h"
#include "CubePlatform.h"
#include "Components.h"
#include "SceneManager.h"
#include "Scene.h"




CubePlatform::CubePlatform()
{
	m_pGameObject = std::make_shared<GameObject>(("Cube"));//+ std::to_string(id)
}

void CubePlatform::Update()
{
	m_pGameObject->Update();
}

void CubePlatform::Render() const
{
	m_pGameObject->Render();
}

void CubePlatform::SwitchColor(const std::string& playerName)
{
	if (m_CurrentGameLevel == GameLevel::Level1)
	{
		if (m_State1)
		{
			m_State1 = false;
			m_State2 = true;
			m_State3 = false;

			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::BlueSecond);

			if (playerName == "Q*Bert")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayer->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}
			else if (playerName == "Q*Bert2")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
				pPlayer->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}

		}
	}
	else if (m_CurrentGameLevel == GameLevel::Level2)
	{
		if (m_State1)
		{
			m_State1 = false;
			m_State2 = true;
			m_State3 = false;

			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::BlueSecond);

			if (playerName == "Q*Bert")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}
			else if (playerName == "Q*Bert2")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}

		}
		else if (m_State2)
		{
			m_State1 = false;
			m_State2 = false;
			m_State3 = true;

			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::GreenThird);

			if (playerName == "Q*Bert")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}
			else if (playerName == "Q*Bert2")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}

		}
	}
	else if (m_CurrentGameLevel == GameLevel::Level3)
	{
		if (m_State1)
		{
			m_State1 = false;
			m_State2 = true;
			m_State3 = false;

			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::BlueSecond);

			if (playerName == "Q*Bert")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}
			else if (playerName == "Q*Bert2")
			{
				auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
				pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::ColorChanged);
			}

		}
		else if (m_State2)
		{
			m_State1 = true;
			m_State2 = false;
			m_State3 = false;

			m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::PinkFirst);

		}
	}
}
