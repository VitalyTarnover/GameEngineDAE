#include "MiniginPCH.h"
#include "CollisionCheckManager.h"
#include "QbertMovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LevelComponent.h"
#include "EnemyManager.h"

void CollisionCheckManager::Update()
{
	SDL_Rect qbertRect = m_pQbert->GetComponent<TransformComponent>()->GetRect();

	if (m_pQbert)
	{

		for (size_t i = 0; i < m_pGameObjectsToCheck.size(); i++)
		{

			if (CheckCollision(qbertRect, m_pGameObjectsToCheck[i]->GetComponent<TransformComponent>()->GetRect()))
			{
				if (m_pGameObjectsToCheck[i]->GetName() == "Disc")
				{
					//snap and move!
					auto qbertMovementComponent = m_pQbert->GetComponent<QbertMovementComponent>();

					if (!qbertMovementComponent->GetIsFallingToDeath())
					{
						qbertMovementComponent->SetDiscTransform(m_pGameObjectsToCheck[i]->GetComponent<TransformComponent>());
						dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->GetDisc(m_pGameObjectsToCheck[i])->SetIsMovingToTop(true, 0);
					}

				}
				else if (m_pGameObjectsToCheck[i]->GetName() == "Ugg"
					|| m_pGameObjectsToCheck[i]->GetName() == "WrongWay"
					|| m_pGameObjectsToCheck[i]->GetName() == "Coily")
				{
					//damage!
					auto currenScene = dae::SceneManager::GetInstance().GetCurrentScene();
					if (currenScene->GetCurrentGameMode() == GameMode::Versus)
					{
						currenScene->GetCurrentLevel()->GetComponent<LevelComponent>()->TeleportPlayersToSpawnPos();
					}
					if (!m_pQbert->GetComponent<QbertMovementComponent>()->GetIsOnDisc())
					{
						auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
						pPlayer->GetComponent<HealthComponent>()->Die();
						
						if (pPlayer->GetComponent<HealthComponent>()->GetLives() > 0)
						{
							pPlayer->GetComponent<QbertMovementComponent>()->LockMovementForSeconds(1.5f);
						}
						else
						{
							pPlayer->GetComponent<QbertMovementComponent>()->SetMovementLocked(true);
							pPlayer->GetComponent<TransformComponent>()->SetPosition(glm::vec3(0, -100, 0));
						}

						EnemyManager::GetInstance().DeleteAllEnemies();
					}

				}
				else if (m_pGameObjectsToCheck[i]->GetName() == "Sam"
					|| m_pGameObjectsToCheck[i]->GetName() == "Slick")
				{
					//bonus!
					auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(0);
					pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::CatchSlickOrSam);

					EnemyManager::GetInstance().DeleteEnemyGameObject(m_pGameObjectsToCheck[i]);

				}
			}
		}
	}

	if (m_pQbert2)
	{
		SDL_Rect qbertRect2 = m_pQbert2->GetComponent<TransformComponent>()->GetRect();

		for (size_t i = 0; i < m_pGameObjectsToCheck.size(); i++)
		{
			if (CheckCollision(qbertRect2, m_pGameObjectsToCheck[i]->GetComponent<TransformComponent>()->GetRect()))
			{
				if (m_pGameObjectsToCheck[i]->GetName() == "Disc")
				{
					auto qbertMovementComponent2 = m_pQbert2->GetComponent<QbertMovementComponent>();

					if (!qbertMovementComponent2->GetIsFallingToDeath())
					{
						m_pQbert2->GetComponent<QbertMovementComponent>()->SetDiscTransform(m_pGameObjectsToCheck[i]->GetComponent<TransformComponent>());
						dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->GetDisc(m_pGameObjectsToCheck[i])->SetIsMovingToTop(true, 1);
					}
				}
				else if (m_pGameObjectsToCheck[i]->GetName() == "Ugg"
					|| m_pGameObjectsToCheck[i]->GetName() == "WrongWay"
					|| m_pGameObjectsToCheck[i]->GetName() == "Coily")
				{
					//damage!
					if (!m_pQbert2->GetComponent<QbertMovementComponent>()->GetIsOnDisc())
					{
						auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
						pPlayer->GetComponent<HealthComponent>()->Die();
						if (pPlayer->GetComponent<HealthComponent>()->GetLives() > 0)
						{
							pPlayer->GetComponent<QbertMovementComponent>()->LockMovementForSeconds(1.5f);
						}
						else
						{
							pPlayer->GetComponent<QbertMovementComponent>()->SetMovementLocked(true);
							pPlayer->GetComponent<TransformComponent>()->SetPosition(glm::vec3(0, -100, 0));
						}
						EnemyManager::GetInstance().DeleteAllEnemies();
					}
				}
				else if (m_pGameObjectsToCheck[i]->GetName() == "Sam"
					|| m_pGameObjectsToCheck[i]->GetName() == "Slick")
				{
					//bonus!
					auto pPlayer = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(1);
					pPlayer.get()->GetComponent<ScoreComponent>()->IncreaseScore((int)Event::CatchSlickOrSam);

					EnemyManager::GetInstance().DeleteEnemyGameObject(m_pGameObjectsToCheck[i]);
				}
			}
		}
	
	}


}

void CollisionCheckManager::AddObjectForCheck(std::shared_ptr<GameObject> gameObject)
{
	if (gameObject->GetName() == "Q*Bert") m_pQbert = gameObject;
	else if (gameObject->GetName() == "Q*Bert2") m_pQbert2 = gameObject;
	else m_pGameObjectsToCheck.push_back(gameObject);
}

void CollisionCheckManager::DeleteGameObject(std::shared_ptr<GameObject> gameObject)
{
	for (size_t i = 0; i < m_pGameObjectsToCheck.size(); i++)
	{
		if (m_pGameObjectsToCheck[i] == gameObject)
		{
			m_pGameObjectsToCheck.erase(std::remove(m_pGameObjectsToCheck.begin(), m_pGameObjectsToCheck.end(), m_pGameObjectsToCheck[i]), m_pGameObjectsToCheck.end());
		}
	}

}

bool CollisionCheckManager::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
	if ((object1.x + object1.w) < object2.x || (object2.x + object2.w) < object1.x)	return false;

	if (object1.y > (object2.y + object2.h) || object2.y > (object1.y + object1.h)) return false;

	return true;
}
