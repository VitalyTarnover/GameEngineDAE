#include "MiniginPCH.h"
#include "CollisionCheckManager.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LevelComponent.h"

void CollisionCheckManager::Update()
{
	auto qbertRect = m_pQbert->GetComponent<TransformComponent>()->GetRect();

	for (auto& otherGameObject : m_pGameObjectsToCheck)
	{
		if (CheckCollision(qbertRect, otherGameObject->GetComponent<TransformComponent>()->GetRect()))
		{
			if (otherGameObject->GetName() == "Disc")
			{
				//snap and move!
				m_pQbert->GetComponent<MovementComponent>()->SetDiscTransform(otherGameObject->GetComponent<TransformComponent>());
				//otherGameObject
				dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentLevel()->GetComponent<LevelComponent>()->GetDisc(0)->SetIsMovingToTop(true);
			}
			else if (otherGameObject->GetName() == "Ugg" 
				|| otherGameObject->GetName() == "WrongWay" 
				|| otherGameObject->GetName() == "Coily")
			{
				//damage!
			}
			else if (otherGameObject->GetName() == "Sam"
				|| otherGameObject->GetName() == "Slick")
			{
				//bonus!
			}
		}
	}

}

void CollisionCheckManager::AddObjectForCheck(std::shared_ptr<GameObject> gameObject)
{
	//auto transform = gameObject->GetComponent<TransformComponent>();

	if (gameObject->GetName() == "Q*Bert") m_pQbert = gameObject;
	else m_pGameObjectsToCheck.push_back(gameObject);
}

bool CollisionCheckManager::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
	if ((object1.x + object1.w) < object2.x || (object2.x + object2.w) < object1.x)	return false;

	if (object1.y > (object2.y + object2.h) || object2.y > (object1.y + object1.h)) return false;

	return true;
}
