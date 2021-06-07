#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "SystemTime.h"
#include "ScoreObserver.h"
#include "LivesObserver.h"
#include "LevelComponent.h"
#include "QbertMovementComponent.h"
#include "EnemyMovementComponent.h"
#include "Commands.h"
#include "AnalogStickCommand.h"
#include "AnalogTriggerCommand.h"
#include "CollisionCheckManager.h"

#include "QbertSceneManager.h"

#include "AudioLocator.h"


using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());


	if (SDL_Init(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());


	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}


	Renderer::GetInstance().Init(m_Window);
	

	AudioLocator::SetAudioService(new AudioServiceProvider());


	//AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::MusicNames::Background01, "Resources/UsoppSiren.mp3");
	//AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::BoomEffect, "Resources/KleerBall01.wav");
	//
	//AudioLocator::GetAudioService().QueueSound(AudioService::MusicNames::Background01, 0.9f);
	//AudioLocator::GetAudioService().QueueSound(AudioService::SoundNames::BoomEffect, 0.9f);


	//AudioServiceProvider* audio = new AudioServiceProvider();
	//
	//AudioLocator::Provide(audio);
	//
	//m_Audio = AudioLocator::GetAudio();
	//
	//m_Audio->AddSoundEffect(AudioService::SoundNames::BoomEffect, "Resources/KleerBall01.wav");
	//
	//m_Audio->PlaySound(AudioService::SoundNames::BoomEffect);

}

void dae::Minigin::LoadGame() const
{
	srand(int(time(NULL)));

	SceneManager::GetInstance().CreateScene("Game");

	QbertSceneManager::GetInstance().LoadMainMenu();

	//scene.SetCurrentGameMode(GameMode::Versus);//GameMode::Coop
	
	//background
	//auto go = std::make_shared<GameObject>("Background");
	//go->AddComponent(new Texture2DComponent("background.jpg"));
	//scene.Add(go);


	//fps counter
	//auto fpsCounter = std::make_shared<GameObject>("FPSCounter");
	//auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	//fpsCounter->AddComponent(new FPSTextComponent(font2));
	//scene.Add(fpsCounter);
	
	
	/*
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	
	switch (scene.GetCurrentGameMode())
	{
	case(GameMode::SinglePlayer):
	{
		//score
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
		auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
		scoreDisplay->AddComponent(scoreCounter);
		scene.Add(scoreDisplay);



		//healths
		auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
		healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
		auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
		healthsDisplay->AddComponent(healthsCounter);
		scene.Add(healthsDisplay);


		auto level = std::make_shared<GameObject>("Level");
		level->AddComponent(new LevelComponent(scene, glm::vec3(630, 175, 0), scene.GetSceneScale()));//670, 200, 0
		scene.Add(level);
		scene.AddLevel(level);
		scene.SetCurrentLevel(level);


		//q*bert
		{
			glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			startPosition.x += scene.GetSceneScale() * 8.f;
			startPosition.y -= scene.GetSceneScale() * 10.f;

			auto qbert = std::make_shared<GameObject>("Q*Bert");
			qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
			qbert->AddComponent(new HealthComponent(3));
			qbert->AddComponent(new ScoreComponent(0));
			qbert->AddWatcher(new LivesObserver());
			qbert->AddWatcher(new ScoreObserver());
			qbert->AddComponent(new Texture2DComponent("Qbert.png", scene.GetSceneScale()));
			qbert->AddComponent(new SpriteAnimComponent(8));
			qbert->AddComponent(new QbertMovementComponent());
			CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
			scene.Add(qbert);
			scene.AddPlayer(qbert);
		}
	}
	break;
	case(GameMode::Coop):
	{
		//score
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
		auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
		scoreDisplay->AddComponent(scoreCounter);
		scene.Add(scoreDisplay);

		auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
		scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(200, 70, 0)));
		auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
		scoreDisplay2->AddComponent(scoreCounter2);
		scene.Add(scoreDisplay2);


		//healths
		auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
		healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
		auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
		healthsDisplay->AddComponent(healthsCounter);
		scene.Add(healthsDisplay);

		auto healthsDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
		healthsDisplay2->AddComponent(new TransformComponent(glm::vec3(200, 100, 0)));
		auto healthsCounter2 = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
		healthsDisplay2->AddComponent(healthsCounter2);
		scene.Add(healthsDisplay2);


		auto level = std::make_shared<GameObject>("Level");
		level->AddComponent(new LevelComponent(scene, glm::vec3(630, 175, 0), scene.GetSceneScale()));//670, 200, 0
		scene.Add(level);
		scene.AddLevel(level);
		scene.SetCurrentLevel(level);


		//q*bert
		{
			glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			startPosition.x += scene.GetSceneScale() * 8.f;
			startPosition.y -= scene.GetSceneScale() * 10.f;

			auto qbert = std::make_shared<GameObject>("Q*Bert");
			qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
			qbert->AddComponent(new HealthComponent(3));
			qbert->AddComponent(new ScoreComponent(0));
			qbert->AddWatcher(new LivesObserver());
			qbert->AddWatcher(new ScoreObserver());
			qbert->AddComponent(new Texture2DComponent("Qbert.png", scene.GetSceneScale()));
			qbert->AddComponent(new SpriteAnimComponent(8));
			qbert->AddComponent(new QbertMovementComponent());
			qbert->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(27);
			CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
			scene.Add(qbert);
			scene.AddPlayer(qbert);
		}
		//q*bert2
		{
			glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(6)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			startPosition.x += scene.GetSceneScale() * 8.f;
			startPosition.y -= scene.GetSceneScale() * 10.f;

			auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
			qbert2->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
			qbert2->AddComponent(new HealthComponent(3));
			qbert2->AddComponent(new ScoreComponent(0));
			qbert2->AddWatcher(new LivesObserver());
			qbert2->AddWatcher(new ScoreObserver());
			qbert2->AddComponent(new Texture2DComponent("Qbert2.png", scene.GetSceneScale()));
			qbert2->AddComponent(new SpriteAnimComponent(8));
			qbert2->AddComponent(new QbertMovementComponent());
			qbert2->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(6);
			CollisionCheckManager::GetInstance().AddObjectForCheck(qbert2);
			scene.Add(qbert2);
			scene.AddPlayer(qbert2);
		}

	}
	break;
	case(GameMode::Versus):
	{
		//score
		auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
		scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
		auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
		scoreDisplay->AddComponent(scoreCounter);
		scene.Add(scoreDisplay);



		//healths
		auto healthsDisplay = std::make_shared<GameObject>("LivesDisplay");
		healthsDisplay->AddComponent(new TransformComponent(glm::vec3(20, 100, 0)));
		auto healthsCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
		healthsDisplay->AddComponent(healthsCounter);
		scene.Add(healthsDisplay);


		auto level = std::make_shared<GameObject>("Level");
		level->AddComponent(new LevelComponent(scene, glm::vec3(630, 175, 0), scene.GetSceneScale()));//670, 200, 0
		scene.Add(level);
		scene.AddLevel(level);
		scene.SetCurrentLevel(level);


		//q*bert
		{
			glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(27)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			startPosition.x += scene.GetSceneScale() * 8.f;
			startPosition.y -= scene.GetSceneScale() * 10.f;

			auto qbert = std::make_shared<GameObject>("Q*Bert");
			qbert->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));//16,21
			qbert->AddComponent(new HealthComponent(3));
			qbert->AddComponent(new ScoreComponent(0));
			qbert->AddWatcher(new LivesObserver());
			qbert->AddWatcher(new ScoreObserver());
			qbert->AddComponent(new Texture2DComponent("Qbert.png", scene.GetSceneScale()));
			qbert->AddComponent(new SpriteAnimComponent(8));
			qbert->AddComponent(new QbertMovementComponent());
			qbert->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(27);
			CollisionCheckManager::GetInstance().AddObjectForCheck(qbert);
			scene.Add(qbert);
			scene.AddPlayer(qbert);
		}
		//coily
		{
			glm::vec3 startPosition = level->GetComponent<LevelComponent>()->GetCube(0)->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();
			startPosition.x += scene.GetSceneScale() * 8.f;
			startPosition.y -= scene.GetSceneScale() * 10.f;

			auto coily = std::make_shared<GameObject>("Coily");
			coily->AddComponent(new TransformComponent(startPosition, glm::vec2{ 15,15 }));
			coily->AddComponent(new Texture2DComponent("Coily.png", scene.GetSceneScale()));

			coily->AddComponent(new QbertMovementComponent(true, 75.f));
			coily->GetComponent<QbertMovementComponent>()->SetCurrentCubeIndex(0);

			coily->AddComponent(new SpriteAnimComponent(8));
			scene.Add(coily);
			CollisionCheckManager::GetInstance().AddObjectForCheck(coily);
			scene.AddPlayer(coily);

		}

	}
	break;
	}
	*/
    




	
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	AudioLocator::Reset();
	Mix_Quit();
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	auto& renderer{ Renderer::GetInstance() };
	auto& sceneManager{ SceneManager::GetInstance() };
	auto& input{ InputManager::GetInstance() };

	bool doContinue{ true };
	auto lastTime{ high_resolution_clock::now() };

	BindCommands();

	std::thread audioThread (&AudioService::Update, &AudioLocator::GetAudioService());



	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();
		input.InputHandler();


		doContinue = input.KeyboardInput();

		SystemTime::GetInstance().SetDeltaTime(deltaTime);
		
		if (SceneManager::GetInstance().GetCurrentScene()->GetCurrentGameMode() != GameMode::MainMenu)
		{
			EnemyManager::GetInstance().Update();
			CollisionCheckManager::GetInstance().Update();
		}

		sceneManager.Update();
		renderer.Render();
	}

	audioThread.detach();
	Cleanup();
}

void dae::Minigin::BindCommands()
{
	auto& input{ InputManager::GetInstance() };
	//assign buttons
	//input.AssignKey<DieCommand>(ControllerButton::ButtonA);
	//input.AssignKey<IncreasePointsCommand>(ControllerButton::ButtonB);
	//input.AssignKey<DieCommand>(ControllerButton::ButtonX, 0);
	//input.AssignKey<IncreasePointsCommand>(ControllerButton::ButtonY, 0);
	//move
	input.AssignKey<JumpUpP2>(ControllerButton::ButtonUp);
	input.AssignKey<JumpDownP2>(ControllerButton::ButtonDown);
	input.AssignKey<JumpLeftP2>(ControllerButton::ButtonLeft);
	input.AssignKey<JumpRightP2>(ControllerButton::ButtonRight);
	//keyboard
	input.AssignKey<JumpUp>(KeyboardButton::W);
	input.AssignKey<JumpDown>(KeyboardButton::S);
	input.AssignKey<JumpLeft>(KeyboardButton::A);
	input.AssignKey<JumpRight>(KeyboardButton::D);
	
	//input.AssignKey<JumpUpP2>(KeyboardButton::I);
	//input.AssignKey<JumpDownP2>(KeyboardButton::K);
	//input.AssignKey<JumpLeftP2>(KeyboardButton::J);
	//input.AssignKey<JumpRightP2>(KeyboardButton::L);

	input.AssignKey<ExitCommand>(KeyboardButton::ESC);

	input.AssignKey<Test1Command>(KeyboardButton::P);
	input.AssignKey<Test2Command>(KeyboardButton::I);

	//input.AssignKey<ExitCommand>(ControllerButton::ButtonSelect);
	//AssignKey<FartCommand>(ControllerButton::ButtonStart);
	//AssignKey<FartCommand>(ControllerButton::ButtonLeftThumb);
	//AssignKey<FartCommand>(ControllerButton::ButtonRightThumb);
	//AssignKey<FartCommand>(ControllerButton::ButtonLeftShoulder);
	//AssignKey<FartCommand>(ControllerButton::ButtonRightShoulder);
	//assign triggers
	//input.AssignTrigger<AimCommand>(m_Triggers[0].first);
	//input.AssignTrigger<ShootCommand>(m_Triggers[1].first);
	//assign sticks
	//input.AssignStick<MoveCommand>(m_Sticks[0].first);
	//input.AssignStick<LookCommand>(m_Sticks[1].first);
}

