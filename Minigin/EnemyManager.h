#pragma once
#include "Singleton.h"
#include <vector>

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:
	EnemyManager() 
	{
		m_CoilySpawnTimer = m_CoilySpawnTime;
		m_WrongWayUggSpawnTimer = m_WrongWayUggSpawnTime;
		m_SamSlickSpawnTimer = m_SamSlickSpawnTime;
	};
	
	void Update();
	
	void SpawnCoily();
	void SpawnWrongWayOrUgg();
	void SpawnSamOrSlick();
	
	void DeleteEnemyGameObject(const std::shared_ptr<GameObject>& gameObject);
	void DeleteAllEnemies(bool timerFullReset = false);
private:
	friend class dae::Singleton<EnemyManager>;

	void DeleteSuicideJumpers();

	float m_CoilySpawnTimer = 0.f;
	float m_CoilySpawnTime = 13.f;

	float m_WrongWayUggSpawnTimer = 0.f;
	float m_WrongWayUggSpawnTime = 6.f;

	float m_SamSlickSpawnTimer = 0.f;
	float m_SamSlickSpawnTime = 20.f;

	std::vector<std::shared_ptr<GameObject>> m_Enemies;
};

