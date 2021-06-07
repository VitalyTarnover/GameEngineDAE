#include "MiniginPCH.h"
#include "QbertSceneManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "LevelComponent.h"
#include "Scene.h"
#include "LivesObserver.h"
#include "ScoreObserver.h"
#include "QbertMovementComponent.h"
#include "EnemyMovementComponent.h"
#include "CollisionCheckManager.h"
#include "EnemyManager.h"

using namespace dae;

void QbertSceneManager::LoadMainMenu()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionCheckManager::GetInstance().CleanUp();
	scene->ClearScene();

	scene->SetCurrentGameMode(GameMode::MainMenu);
	scene->SetGameLevel(GameLevel::Level1);

	//background
	auto background = std::make_shared<GameObject>("Background");
	background->AddComponent(new Texture2DComponent("QbertBackground.jpg"));
	scene->Add(background);

	auto text1 = std::make_shared<GameObject>("Text1");
	text1->AddComponent(new TransformComponent(glm::vec3(20, 150, 0)));
	auto textText1 = new TextComponent("Q*Bert!", font, SDL_Color{ 255,165,0 });
	text1->AddComponent(textText1);
	scene->Add(text1);

	auto text2 = std::make_shared<GameObject>("Text2");
	text2->AddComponent(new TransformComponent(glm::vec3(20, 200, 0)));
	auto textText2 = new TextComponent("Controls:", font, SDL_Color{ 255,255,255 });
	text2->AddComponent(textText2);
	scene->Add(text2);

	auto text3 = std::make_shared<GameObject>("Text3");
	text3->AddComponent(new TransformComponent(glm::vec3(20, 225, 0)));
	auto textText3 = new TextComponent("Player1(WASD/Gamepad): WD = Jump left up, SD = Jump left down, AS = Jump right down, WA = Jump right up", font, SDL_Color{ 255,255,255 });
	text3->AddComponent(textText3);
	scene->Add(text3);

	auto text4 = std::make_shared<GameObject>("Text4");
	text4->AddComponent(new TransformComponent(glm::vec3(20, 250, 0)));
	auto textText4 = new TextComponent("Player2(IJKL): IL = Jump left up, LK = Jump left down, KJ = Jump right down, IJ = Jump right up", font, SDL_Color{ 255,255,255 });
	text4->AddComponent(textText4);
	scene->Add(text4);

	auto text5 = std::make_shared<GameObject>("Text5");
	text5->AddComponent(new TransformComponent(glm::vec3(20, 350, 0)));
	auto textText5 = new TextComponent("Game modes:", font, SDL_Color{ 255,255,255 });
	text5->AddComponent(textText5);
	scene->Add(text5);


	auto text6 = std::make_shared<GameObject>("Text6");
	text6->AddComponent(new TransformComponent(glm::vec3(20, 375, 0)));
	auto textText6 = new TextComponent("Press 1 to play SinglePlayer mode", font, SDL_Color{ 255,255,255 });
	text6->AddComponent(textText6);
	scene->Add(text6);

	auto text7 = std::make_shared<GameObject>("Text7");
	text7->AddComponent(new TransformComponent(glm::vec3(20, 400, 0)));
	auto textText7 = new TextComponent("Press 2 to play Co-op mode", font, SDL_Color{ 255,255,255 });
	text7->AddComponent(textText7);
	scene->Add(text7);

	auto text8 = std::make_shared<GameObject>("Text8");
	text8->AddComponent(new TransformComponent(glm::vec3(20, 425, 0)));
	auto textText8 = new TextComponent("Press 3 to play Versus mode", font, SDL_Color{ 255,255,255 });
	text8->AddComponent(textText8);
	scene->Add(text8);

	auto text9 = std::make_shared<GameObject>("Text9");
	text9->AddComponent(new TransformComponent(glm::vec3(20, 470, 0)));
	auto textText9 = new TextComponent("While in game, press P to return to this main menu", font, SDL_Color{ 255,255,255 });
	text9->AddComponent(textText9);
	scene->Add(text9);


	auto text10 = std::make_shared<GameObject>("Text9");
	text10->AddComponent(new TransformComponent(glm::vec3(20, 500, 0)));
	auto textText10 = new TextComponent("Press ESC to close the game", font, SDL_Color{ 255,255,255 });
	text10->AddComponent(textText10);
	scene->Add(text10);

}

void QbertSceneManager::LoadSinglePlayer()
{

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scene = SceneManager::GetInstance().GetCurrentScene();
	
	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionCheckManager::GetInstance().CleanUp();
	scene->ClearScene();

	scene->SetCurrentGameMode(GameMode::SinglePlayer);
	scene->SetGameLevel(GameLevel::Level1);

	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);



	//healths
	auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
	auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	healthsDisplay->AddComponent(healthsCounter);
	scene->Add(healthsDisplay);


	auto level = std::make_shared<GameObject>("Level");
	level->AddComponent(new LevelComponent(*scene, glm::vec3(630, 175, 0), scene->GetSceneScale()));//670, 200, 0
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);


	//q*bert
	{
		glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPosition.x += scene->GetSceneScale() * 8.f;
		startPosition.y -= scene->GetSceneScale() * 10.f;

		auto qbert = std::make_shared<GameObject>("Q*Bert");
		qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
		qbert->AddComponent(new HealthComponent(3));
		qbert->AddComponent(new ScoreComponent(0));
		qbert->AddWatcher(new LivesObserver());
		qbert->AddWatcher(new ScoreObserver());
		qbert->AddComponent(new Texture2DComponent("Qbert.png", scene->GetSceneScale()));
		qbert->AddComponent(new SpriteAnimComponent(8));
		qbert->AddComponent(new QbertMovementComponent());
		CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
		scene->Add(qbert);
		scene->AddPlayer(qbert);
	}



}

void QbertSceneManager::LoadCoop()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionCheckManager::GetInstance().CleanUp();
	scene->ClearScene();

	scene->SetCurrentGameMode(GameMode::Coop);
	scene->SetGameLevel(GameLevel::Level1);

	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);

	auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
	scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(200, 70, 0)));
	auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay2->AddComponent(scoreCounter2);
	scene->Add(scoreDisplay2);


	//healths
	auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
	auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	healthsDisplay->AddComponent(healthsCounter);
	scene->Add(healthsDisplay);

	auto healthsDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
	healthsDisplay2->AddComponent(new TransformComponent(glm::vec3(200, 100, 0)));
	auto healthsCounter2 = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	healthsDisplay2->AddComponent(healthsCounter2);
	scene->Add(healthsDisplay2);


	auto level = std::make_shared<GameObject>("Level");
	level->AddComponent(new LevelComponent(*scene, glm::vec3(630, 175, 0), scene->GetSceneScale()));//670, 200, 0
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);


	//q*bert
	{
		glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPosition.x += scene->GetSceneScale() * 8.f;
		startPosition.y -= scene->GetSceneScale() * 10.f;
								
		auto qbert = std::make_shared<GameObject>("Q*Bert");
		qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
		qbert->AddComponent(new HealthComponent(3));
		qbert->AddComponent(new ScoreComponent(0));
		qbert->AddWatcher(new LivesObserver());
		qbert->AddWatcher(new ScoreObserver());
		qbert->AddComponent(new Texture2DComponent("Qbert.png", scene->GetSceneScale()));
		qbert->AddComponent(new SpriteAnimComponent(8));
		qbert->AddComponent(new QbertMovementComponent());
		qbert->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(27);
		CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
		scene->Add(qbert);
		scene->AddPlayer(qbert);
	}
	//q*bert2
	{
		glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPosition.x += scene->GetSceneScale() * 8.f;
		startPosition.y -= scene->GetSceneScale() * 10.f;

		auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
		qbert2->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
		qbert2->AddComponent(new HealthComponent(3));
		qbert2->AddComponent(new ScoreComponent(0));
		qbert2->AddWatcher(new LivesObserver());
		qbert2->AddWatcher(new ScoreObserver());
		qbert2->AddComponent(new Texture2DComponent("Qbert2.png", scene->GetSceneScale()));
		qbert2->AddComponent(new SpriteAnimComponent(8));
		qbert2->AddComponent(new QbertMovementComponent());
		qbert2->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(6);
		CollisionCheckManager::GetInstance().AddObjectForCheck(qbert2);
		scene->Add(qbert2);
		scene->AddPlayer(qbert2);
	}


}

void QbertSceneManager::LoadVersus()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto scene = SceneManager::GetInstance().GetCurrentScene();

	EnemyManager::GetInstance().DeleteAllEnemies();
	CollisionCheckManager::GetInstance().CleanUp();
	scene->ClearScene();

	scene->SetCurrentGameMode(GameMode::Versus);
	scene->SetGameLevel(GameLevel::Level1);


	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene->Add(scoreDisplay);



	//healths
	auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
	healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
	auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	healthsDisplay->AddComponent(healthsCounter);
	scene->Add(healthsDisplay);


	auto level = std::make_shared<GameObject>("Level");
	level->AddComponent(new LevelComponent(*scene, glm::vec3(630, 175, 0), scene->GetSceneScale()));//670, 200, 0
	scene->Add(level);
	scene->AddLevel(level);
	scene->SetCurrentLevel(level);


	//q*bert
	{
		glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPosition.x += scene->GetSceneScale() * 8.f;
		startPosition.y -= scene->GetSceneScale() * 10.f;

		auto qbert = std::make_shared<GameObject>("Q*Bert");
		qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
		qbert->AddComponent(new HealthComponent(3));
		qbert->AddComponent(new ScoreComponent(0));
		qbert->AddWatcher(new LivesObserver());
		qbert->AddWatcher(new ScoreObserver());
		qbert->AddComponent(new Texture2DComponent("Qbert.png", scene->GetSceneScale()));
		qbert->AddComponent(new SpriteAnimComponent(8));
		qbert->AddComponent(new QbertMovementComponent());
		qbert->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(27);
		CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
		scene->Add(qbert);
		scene->AddPlayer(qbert);
	}
	//coily
	{
		glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
		startPosition.x += scene->GetSceneScale() * 8.f;
		startPosition.y -= scene->GetSceneScale() * 10.f;

		auto coily = std::make_shared<GameObject>("Coily");
		coily->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));
		coily->AddComponent(new Texture2DComponent("Coily.png", scene->GetSceneScale()));

		coily->AddComponent(new QbertMovementComponent(true, 75.f));
		coily->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(0);

		coily->AddComponent(new SpriteAnimComponent(8));
		scene->Add(coily);
		CollisionCheckManager::GetInstance().AddObjectForCheck(coily);
		scene->AddPlayer(coily);

	}
}
