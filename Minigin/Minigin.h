#pragma once


//class AudioServiceProvider;
//class AudioService;

struct SDL_Window;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		void LoadGame() const;
		
		void LoadMenuScene();
		
		void Cleanup();
		void Run();
		void BindCommands();
	private:


		static const int m_MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		const float m_MsPerUpdate = m_MsPerFrame / 1000.0f; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};

		//AudioService* m_Audio;
	};
}