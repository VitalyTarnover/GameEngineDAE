#include "MiniginPCH.h"
#include "Components.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "EnemyMovementComponent.h"
#include "LevelComponent.h"
#include "LivesObserver.h"
#include "CollisionCheckManager.h"
#include "Scene.h"
#include "Minigin.h"

using namespace dae;


void EnemyManager::Update(float deltaT)
{
	m_SpawnTimer += deltaT;

	if (m_SpawnTimer >= m_SpawnEnemyInterval)
	{
		m_SpawnTimer = 0.0f;
		SpawnWrongWay();
	}
}

void EnemyManager::SpawnWrongWay()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto level = scene->GetCurrentLevel();

	
	const int randNr = rand() % 2;
	TransformComponent* transformComponent;
	if (randNr == 0)
	{
		int enemyWidth = 15;
		int enemyHeight = 20;

		glm::vec3 leftCubePos = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		transformComponent = new TransformComponent(glm::vec3(leftCubePos.x - enemyWidth , leftCubePos.y + enemyHeight * 1.5f ,0), glm::vec2(enemyWidth, enemyHeight));

		auto wrongWay = std::make_shared<GameObject>("WrongWay");
		wrongWay->AddComponent(transformComponent);
		wrongWay->AddComponent(new HealthComponent(1));
		wrongWay->AddWatcher(new LivesObserver());
		wrongWay->AddComponent(new Texture2DComponent("WrongWay.png", scene->GetSceneScale()));
		wrongWay->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::WrongWay, true));
		wrongWay->AddComponent(new SpriteAnimComponent(8));
		scene->Add(wrongWay);
		CollisionCheckManager::GetInstance().AddObjectForCheck(wrongWay);
		m_WrongWayCount++;
	}
	else
	{
		int offset = 50;

		glm::vec3 rightCubePos = level->GetComponent<LevelComponent>()->GetCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		transformComponent = new TransformComponent(glm::vec3(rightCubePos.x + offset, rightCubePos.y + offset,0), glm::vec2(15, 20));

		auto wrongWay = std::make_shared<GameObject>("Ugg");
		wrongWay->AddComponent(transformComponent);
		wrongWay->AddComponent(new HealthComponent(1));
		wrongWay->AddWatcher(new LivesObserver());
		wrongWay->AddComponent(new Texture2DComponent("Ugg.png", scene->GetSceneScale()));
		wrongWay->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Ugg, false));
		wrongWay->AddComponent(new SpriteAnimComponent(8));
		wrongWay->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::EnemyOnPlatformLeftDown);
		scene->Add(wrongWay);
		CollisionCheckManager::GetInstance().AddObjectForCheck(wrongWay);
		m_WrongWayCount++;

	}
	

	
}

void EnemyManager::SpawnCoily()
{

	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto level = scene->GetCurrentLevel();

	glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	startPosition.x += scene->GetSceneScale() * 8.f;
	startPosition.y -= scene->GetSceneScale() * 10.f;

	auto coily = std::make_shared<GameObject>("Coily");
	coily->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));
	coily->AddComponent(new HealthComponent(1));
	coily->AddWatcher(new LivesObserver());
	coily->AddComponent(new Texture2DComponent("Coily.png", scene->GetSceneScale()));//("WrongWay.png", 2, true)
	coily->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Coily));
	coily->AddComponent(new SpriteAnimComponent(8));
	scene->Add(coily);
	CollisionCheckManager::GetInstance().AddObjectForCheck(coily);
	m_CoilyCount++;
}

void EnemyManager::RemoveEnemyByName(const std::string&)
{
	//auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	//auto& collisionManager = CollisionManager::GetInstance();
	//currentScene->RemoveObjectsByName(name);
	//collisionManager.RemoveObjectsByName(name);
	//
	//m_CoilyCount--;
}


void EnemyManager::ClearEnemies()
{
	//auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	//auto& collisionManager = CollisionManager::GetInstance();
	//currentScene->RemoveObjectsByTag(dae::Tag::Coily);
	//collisionManager.RemoveCollidersByTag(dae::Tag::Coily);
	//
	//m_CoilyCount = 0;
}

