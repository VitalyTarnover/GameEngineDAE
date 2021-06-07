#include "MiniginPCH.h"
#include "Components.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "EnemyMovementComponent.h"
#include "LevelComponent.h"
#include "CollisionCheckManager.h"
#include "Scene.h"
#include "Minigin.h"
#include "QbertSceneManager.h"


using namespace dae;

void EnemyManager::Update()
{
	auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();
	float elapsedTime = SystemTime::GetInstance().GetDeltaTime();


	if (currentScene->GetCurrentGameMode() == GameMode::SinglePlayer)
	{
		if (!currentScene->GetPlayer(0) || currentScene->GetPlayer(0)->GetComponent<HealthComponent>()->GetLives() <= 0)
		{
			if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != GameMode::MainMenu)
				QbertSceneManager::GetInstance().LoadMainMenu();
			return;
		}
	}
	else if (currentScene->GetCurrentGameMode() == GameMode::Coop)
	{
		if (!currentScene->GetPlayer(0) || currentScene->GetPlayer(0)->GetComponent<HealthComponent>()->GetLives() <= 0
			&& (!currentScene->GetPlayer(1) || currentScene->GetPlayer(1)->GetComponent<HealthComponent>()->GetLives() <= 0))
		{
			if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != GameMode::MainMenu)
				QbertSceneManager::GetInstance().LoadMainMenu();
			return;
		}
	}
	else if (currentScene->GetCurrentGameMode() == GameMode::Versus)
	{

		if (!currentScene->GetPlayer(0) || currentScene->GetPlayer(0)->GetComponent<HealthComponent>()->GetLives() <= 0)
		{
			if (dae::SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != GameMode::MainMenu)
				QbertSceneManager::GetInstance().LoadMainMenu();
			return;
		}

		return;
	}



	if (m_Enemies.size() <= 5)
	{
		m_CoilySpawnTimer -= elapsedTime;
		m_WrongWayUggSpawnTimer -= elapsedTime;
		m_SamSlickSpawnTimer -= elapsedTime;


		if (m_CoilySpawnTimer <= 0.f)
		{
			SpawnCoily();
			m_CoilySpawnTimer = m_CoilySpawnTime;
		}

		if (m_WrongWayUggSpawnTimer <= 0.f)
		{
			SpawnWrongWayOrUgg();
			m_WrongWayUggSpawnTimer = m_WrongWayUggSpawnTime;
		}

		if (m_SamSlickSpawnTimer <= 0.f)
		{
			SpawnSamOrSlick();
			m_SamSlickSpawnTimer = m_SamSlickSpawnTime;
		}
	}

	DeleteSuicideJumpers();
}

void EnemyManager::SpawnWrongWayOrUgg()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto level = scene->GetCurrentLevel();
	
	int spawnOnRigth = rand() % 2;

	if (spawnOnRigth == 0)
	{
		int enemyWidth = 15;
		int enemyHeight = 20;

		glm::vec3 leftCubePos = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		TransformComponent* transformComponent = new TransformComponent(glm::vec3(leftCubePos.x - enemyWidth , leftCubePos.y + enemyHeight * 1.5f ,0), glm::vec2(enemyWidth, enemyHeight));

		auto wrongWay = std::make_shared<GameObject>("WrongWay");
		wrongWay->AddComponent(transformComponent);
		wrongWay->AddComponent(new Texture2DComponent("WrongWay.png", scene->GetSceneScale()));

		wrongWay->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::WrongWay));

		wrongWay->AddComponent(new SpriteAnimComponent(8));
		scene->Add(wrongWay);
		CollisionCheckManager::GetInstance().AddObjectForCheck(wrongWay);
		m_Enemies.push_back(wrongWay);
	}
	else
	{
		int offset = 50;
		glm::vec3 rightCubePos = level->GetComponent<LevelComponent>()->GetCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		TransformComponent* transformComponent = new TransformComponent(glm::vec3(rightCubePos.x + offset, rightCubePos.y + offset,0), glm::vec2(25, 20));

		auto ugg = std::make_shared<GameObject>("Ugg");
		ugg->AddComponent(transformComponent);
		ugg->AddComponent(new Texture2DComponent("Ugg.png", scene->GetSceneScale()));

		ugg->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Ugg));

		ugg->AddComponent(new SpriteAnimComponent(8));
		ugg->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftDown);
		scene->Add(ugg);
		CollisionCheckManager::GetInstance().AddObjectForCheck(ugg);
		m_Enemies.push_back(ugg);
	}
}

void EnemyManager::SpawnSamOrSlick()
{
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto level = scene->GetCurrentLevel();

	int spawnOnRigth = rand() % 2;

	if (spawnOnRigth == 0)
	{
		glm::vec3 rightCubePos = level->GetComponent<LevelComponent>()->GetCube(1)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		rightCubePos.x += scene->GetSceneScale() * 8.f;
		rightCubePos.y -= scene->GetSceneScale() * 10.f;

		auto slick = std::make_shared<GameObject>("Slick");
		slick->AddComponent(new TransformComponent(rightCubePos, glm::vec2{ 15,15 }));
		slick->AddComponent(new Texture2DComponent("Slick.png", scene->GetSceneScale()));
		slick->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Slick, 125.f, 0.75f));
		slick->AddComponent(new SpriteAnimComponent(8));
		slick->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftDown);
		scene->Add(slick);
		m_Enemies.push_back(slick);

		CollisionCheckManager::GetInstance().AddObjectForCheck(slick);
	}
	else
	{
		glm::vec3 leftCubePos = level->GetComponent<LevelComponent>()->GetCube(7)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

		leftCubePos.x += scene->GetSceneScale() * 8.f;
		leftCubePos.y -= scene->GetSceneScale() * 10.f;
		
		auto sam = std::make_shared<GameObject>("Sam");
		sam->AddComponent(new TransformComponent(leftCubePos, glm::vec2{ 15,15 }));
		sam->AddComponent(new Texture2DComponent("Sam.png", scene->GetSceneScale()));
		sam->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Sam, 125.f, 0.75f));
		sam->AddComponent(new SpriteAnimComponent(8));
		sam->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::OnPlatformLeftDown);
		scene->Add(sam);
		m_Enemies.push_back(sam);

		CollisionCheckManager::GetInstance().AddObjectForCheck(sam);
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
	coily->AddComponent(new Texture2DComponent("Coily.png", scene->GetSceneScale()));

	if (scene->GetPlayer(1) != nullptr) coily->AddComponent(new EnemyMovementComponent(scene->GetPlayer(rand() % 2), EnemyMovementComponent::EnemyType::Coily));
	else coily->AddComponent(new EnemyMovementComponent(scene->GetPlayer(0), EnemyMovementComponent::EnemyType::Coily, 120.f, 1.f));

	
	coily->AddComponent(new SpriteAnimComponent(8));
	scene->Add(coily);
	CollisionCheckManager::GetInstance().AddObjectForCheck(coily);
	m_Enemies.push_back(coily);
}

void EnemyManager::DeleteEnemyGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	auto currentScene = SceneManager::GetInstance().GetCurrentScene();
	auto& collisionManager = CollisionCheckManager::GetInstance();
	
	currentScene->DeleteGameObject(gameObject);
	collisionManager.DeleteGameObject(gameObject);

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), m_Enemies[i]), m_Enemies.end());
	}
	
}


void EnemyManager::DeleteAllEnemies(bool timerFullReset)
{
	auto& collisionManager = CollisionCheckManager::GetInstance();

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		m_Enemies[i]->SetMarkedForDelete(true);
		collisionManager.DeleteGameObject(m_Enemies[i]);
	}
	m_Enemies.clear();

	if (timerFullReset)
	{
		m_CoilySpawnTimer = m_CoilySpawnTime / 2;
		m_WrongWayUggSpawnTimer = m_WrongWayUggSpawnTime / 2;
		m_SamSlickSpawnTimer = m_SamSlickSpawnTime;
	}
	else
	{
		m_CoilySpawnTimer = m_CoilySpawnTime;
		m_WrongWayUggSpawnTimer = m_WrongWayUggSpawnTime;
		m_SamSlickSpawnTimer = m_SamSlickSpawnTime;
	}

}

void EnemyManager::DeleteSuicideJumpers()
{
	auto& collisionManager = CollisionCheckManager::GetInstance();

	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i]->GetComponent<EnemyMovementComponent>()->GetIsInDeathZone())
		{
			m_Enemies[i]->SetMarkedForDelete(true);
			collisionManager.DeleteGameObject(m_Enemies[i]);
			m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), m_Enemies[i]), m_Enemies.end());
		}
	}
}

