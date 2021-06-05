#pragma once
#include "Singleton.h"

class EnemyManager : public dae::Singleton<EnemyManager>
{
public:
	EnemyManager() {};
	void Update(float deltaT);
	void SpawnCoily();
	void SpawnWrongWay();
	void ClearEnemies();
	void RemoveEnemyByName(const std::string& Enemy);
private:
	friend class dae::Singleton<EnemyManager>;

	float m_SpawnTimer = 0.f;
	float m_SpawnEnemyInterval = 7.5f;
	int m_CoilyCount = 0;
	int m_WrongWayCount = 0;
};

