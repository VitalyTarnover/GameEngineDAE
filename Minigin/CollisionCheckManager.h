#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "Components.h"

//class TransformComponent;

class CollisionCheckManager final : public dae::Singleton<CollisionCheckManager>
{
public:
	void Update();
	void AddObjectForCheck(std::shared_ptr<GameObject> gameObject);
	void DeleteGameObject(std::shared_ptr<GameObject> gameObject);
private:
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	
	CollisionCheckManager() = default;
	friend class dae::Singleton<CollisionCheckManager>;
	
	std::vector<std::shared_ptr<GameObject>> m_pGameObjectsToCheck;
	std::shared_ptr<GameObject> m_pQbert;
	std::shared_ptr<GameObject> m_pQbert2;
};
