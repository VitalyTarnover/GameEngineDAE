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
#include "Time.h"
#include "ScoreObserver.h"
#include "LivesObserver.h"
#include <thread>

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
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}


	Renderer::GetInstance().Init(m_Window);
	

	AudioLocator::SetAudioService(new AudioServiceProvider());


	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::MusicNames::Background01, "Resources/UsoppSiren.mp3");
	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::BoomEffect, "Resources/KleerBall01.wav");

	AudioLocator::GetAudioService().QueueSound(AudioService::MusicNames::Background01, 0.9f);
	AudioLocator::GetAudioService().QueueSound(AudioService::SoundNames::BoomEffect, 0.9f);


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

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	
	//background
	auto go = std::make_shared<GameObject>("Background");
	go->AddComponent(new Texture2DComponent("background.jpg"));
	scene.Add(go);
	
	//logo
	go = std::make_shared<GameObject>("Logo");
	go->AddComponent(new TransformComponent(glm::vec3{ 216,180,0 }));
	go->AddComponent(new Texture2DComponent("logo.png"));
	scene.Add(go);
	
	//titel
	//go = std::make_shared<GameObject>("Titel");
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//go->AddComponent(new TransformComponent(glm::vec3{ 100,430,0 }));
	//go->AddComponent(new TextComponent("Programming 4 Assignment", font, SDL_Color{ 255,255,255 }));
	//scene.Add(go);
	
	//fps counter
	go = std::make_shared<GameObject>("FPSCounter");
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	go->AddComponent(new FPSTextComponent(font2));
	scene.Add(go);
	
	
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	//Player1
		
	//titel
	auto player1Titel = std::make_shared<GameObject>("Player 1 titel");
	player1Titel->AddComponent(new TransformComponent(glm::vec3(20, 20, 0)));
	player1Titel->AddComponent(new TextComponent("Player 1", font, SDL_Color{ 255,25,25 }));
	scene.Add(player1Titel);

	//lives
	auto livesDisplay = std::make_shared<GameObject>("LivesDisplay");
	livesDisplay->AddComponent(new TransformComponent(glm::vec3(20, 50, 0)));
	auto livesCounter = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay->AddComponent(livesCounter);
	scene.Add(livesDisplay);
	
	//score
	auto scoreDisplay = std::make_shared<GameObject>("ScoreDisplay");
	scoreDisplay->AddComponent(new TransformComponent(glm::vec3(20, 70, 0)));
	auto scoreCounter = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay->AddComponent(scoreCounter);
	scene.Add(scoreDisplay);
	
	//q*bert
	auto qbert = std::make_shared<GameObject>("Q*Bert");
	qbert->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	qbert->AddComponent(new HealthComponent(3));
	qbert->AddComponent(new ScoreComponent(0));
	qbert->AddWatcher(new LivesObserver());
	qbert->AddWatcher(new ScoreObserver());
	scene.Add(qbert);
	scene.AddPlayer(qbert);

	//player died text
	auto playerDied = std::make_shared<GameObject>("Player 1 Died!");
	playerDied->AddComponent(new TransformComponent(glm::vec3(250, 300, 0)));
	playerDied->AddComponent(new TextComponent("Player 1 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene.Add(playerDied);
	
	//Player2

	//titel
	auto player2Titel = std::make_shared<GameObject>("Player 2 titel");
	player2Titel->AddComponent(new TransformComponent(glm::vec3(500, 20, 0)));
	player2Titel->AddComponent(new TextComponent("Player 2",font, SDL_Color{ 255,25,25 }));
	scene.Add(player2Titel);

	//lives
	auto livesDisplay2 = std::make_shared<GameObject>("LivesDisplay2");
	livesDisplay2->AddComponent(new TransformComponent(glm::vec3(500, 50, 0)));
	auto livesCounter2 = new TextComponent("Lives: 3", font, SDL_Color{ 255,255,255 });
	livesDisplay2->AddComponent(livesCounter2);
	scene.Add(livesDisplay2);

	//score
	auto scoreDisplay2 = std::make_shared<GameObject>("ScoreDisplay2");
	scoreDisplay2->AddComponent(new TransformComponent(glm::vec3(500, 70, 0)));
	auto scoreCounter2 = new TextComponent("Score: 0", font, SDL_Color{ 255,255,255 });
	scoreDisplay2->AddComponent(scoreCounter2);
	scene.Add(scoreDisplay2);
	//q*bert

	auto qbert2 = std::make_shared<GameObject>("Q*Bert2");
	qbert2->AddComponent(new TransformComponent(glm::vec3(0, 0, 0)));
	qbert2->AddComponent(new HealthComponent(3));
	qbert2->AddComponent(new ScoreComponent(0));
	qbert2->AddWatcher(new LivesObserver());
	qbert2->AddWatcher(new ScoreObserver());
	scene.Add(qbert2);
	scene.AddPlayer(qbert2);


	//player died text
	auto playerDied2 = std::make_shared<GameObject>("Player 2 Died!");
	playerDied2->AddComponent(new TransformComponent(glm::vec3(250, 100, 0)));
	playerDied2->AddComponent(new TextComponent("Player 2 Died!", font, SDL_Color{ 255,255,255 }, false));
	scene.Add(playerDied2);

	////How to play
	auto howToPlay1 = std::make_shared<GameObject>("How to play");
	howToPlay1->AddComponent(new TransformComponent(glm::vec3(120, 380, 0)));
	howToPlay1->AddComponent(new TextComponent("A - kill Player 1, B - +25 to score of Player 1", font, SDL_Color{ 100,255,100 }));
	scene.Add(howToPlay1);

	auto howToPlay2 = std::make_shared<GameObject>("How to play");
	howToPlay2->AddComponent(new TransformComponent(glm::vec3(120, 400, 0)));
	howToPlay2->AddComponent(new TextComponent("X - kill Player 2, Y - +25 to score of Player 2", font, SDL_Color{ 100,255,100 }));
	scene.Add(howToPlay2);

	auto howToPlay3 = std::make_shared<GameObject>("How to play");
	howToPlay3->AddComponent(new TransformComponent(glm::vec3(120, 420, 0)));
	howToPlay3->AddComponent(new TextComponent("Back - exit the game", font, SDL_Color{ 100,255,100 }));
	scene.Add(howToPlay3);

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

	input.BindCommands();

	std::thread audioThread (&AudioService::Update, &AudioLocator::GetAudioService());

	while (doContinue)
	{
		const auto currentTime{ high_resolution_clock::now() };
		const float deltaTime{ duration<float>(currentTime - lastTime).count() };
		lastTime = currentTime;

		input.ProcessInput();
		input.ControllerAnalogs();

		doContinue = input.InputHandler();

		Time::GetInstance().SetDeltaTime(deltaTime);

		sceneManager.Update();
		renderer.Render();
	}

	audioThread.detach();
	Cleanup();
}