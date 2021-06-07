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
#include "AudioServiceProvider.h"

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
	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::DiscSFX, "Resources/SFX/QbertDiscSFX.wav");
	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::FallSFX, "Resources/SFX/QbertFallSFX.wav");
	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::JumpSFX, "Resources/SFX/QbertJumpSFX.wav");
	AudioLocator::GetAudioService().AddSoundToLibrary(AudioService::SoundNames::LevelCompleteSFX, "Resources/SFX/QbertLevelCompleteSFX.wav");

	//AudioLocator::GetAudioService().QueueSound(AudioService::MusicNames::Background01, 0.9f);



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

	AudioLocator::GetAudioService().QueueSound(AudioService::SoundNames::LevelCompleteSFX, 30.f);

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
	auto& inputManager = InputManager::GetInstance();

	//keyboard
	inputManager.AssignKey<JumpUp>(KeyboardButton::W, 0);
	inputManager.AssignKey<JumpDown>(KeyboardButton::S, 0);
	inputManager.AssignKey<JumpLeft>(KeyboardButton::A, 0);
	inputManager.AssignKey<JumpRight>(KeyboardButton::D, 0);
	inputManager.AssignKey<ExitCommand>(KeyboardButton::ESC, 0);


	inputManager.AssignKey<JumpUp>(KeyboardButton::I, 1);
	inputManager.AssignKey<JumpDown>(KeyboardButton::K, 1);
	inputManager.AssignKey<JumpLeft>(KeyboardButton::J, 1);
	inputManager.AssignKey<JumpRight>(KeyboardButton::L, 1);

	inputManager.AssignKey<LoadSinglePlayerCommand>(KeyboardButton::One, 0);
	inputManager.AssignKey<LoadCoopCommand>(KeyboardButton::Two, 0);
	inputManager.AssignKey<LoadVersusCommand>(KeyboardButton::Three, 0);
	inputManager.AssignKey<LoadMainMenuCommand>(KeyboardButton::P, 0);

	//inputManager.AssignKey<Test1Command>(KeyboardButton::P, 0);

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		//gamepad
		inputManager.AssignKey<JumpUp>(ControllerButton::ButtonUp, i);
		inputManager.AssignKey<JumpDown>(ControllerButton::ButtonDown, i);
		inputManager.AssignKey<JumpLeft>(ControllerButton::ButtonLeft, i);
		inputManager.AssignKey<JumpRight>(ControllerButton::ButtonRight, i);

	}

}
