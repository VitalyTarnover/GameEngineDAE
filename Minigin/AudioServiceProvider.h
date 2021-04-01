#pragma once
#include "AudioService.h"
#include <SDL_mixer.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <map>


class AudioServiceProvider final :  public AudioService
{
public:
	AudioServiceProvider()
	{
		m_Playing.store(true);
	};

	~AudioServiceProvider()
	{
		m_Playing.store(false);
		m_QueueActive.notify_one();

		if (m_MusicLibrary.find(MusicNames::Background01) != m_MusicLibrary.end())
			Mix_FreeMusic(m_MusicLibrary.at(MusicNames::Background01));
		if (m_MusicLibrary.find(MusicNames::Background02) != m_MusicLibrary.end())
			Mix_FreeMusic(m_MusicLibrary.at(MusicNames::Background02));
		if (m_SoundLibrary.find(SoundNames::BoomEffect) != m_SoundLibrary.end())
			Mix_FreeChunk(m_SoundLibrary.at(SoundNames::BoomEffect));
		if (m_SoundLibrary.find(SoundNames::ChargeEffect) != m_SoundLibrary.end())
			Mix_FreeChunk(m_SoundLibrary.at(SoundNames::ChargeEffect));

	};

	void QueueSound(const SoundNames& key, float volume = 1.0f) override
	{
		SoundUnit sound{ int(key),volume,false };
		std::lock_guard<std::mutex> mLock{ m_Mutex };
		m_SoundQueue.push(sound);
		m_QueueActive.notify_one();
	}

	void QueueSound(const MusicNames& key, float volume = 1.0f) override
	{
		SoundUnit sound{ int(key),volume,true };
		std::lock_guard<std::mutex> mLock{ m_Mutex };
		m_SoundQueue.push(sound);
		m_QueueActive.notify_one();
	}

	void AddSoundToLibrary(const SoundNames& soundId, const std::string& path) override
	{
		m_SoundLibrary.insert(std::pair<SoundNames, Mix_Chunk*>(soundId, Mix_LoadWAV(path.c_str())));
	}

	void AddSoundToLibrary(const MusicNames& soundId, const std::string& path) override
	{
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		m_MusicLibrary.insert(std::pair<MusicNames, Mix_Music*>(soundId, music));
	}

	void Update() override
	{
		while (m_Playing.load())
		{
			while (!m_SoundQueue.empty())
			{
				std::unique_lock<std::mutex> mLock{ m_Mutex };
				SoundUnit ps = m_SoundQueue.front();
				m_SoundQueue.pop();

				if (ps.music)
					Play(MusicNames(ps.id), ps.volume);
				else
					Play(SoundNames(ps.id), ps.volume);
			}
			std::unique_lock<std::mutex> guard{ m_Mutex };
			m_QueueActive.wait(guard);
		}
	}

	void Pause() override
	{
		if (Mix_PausedMusic())
			Start();
		else
			Stop();
	}

protected:

	void Play(const SoundNames& key, float volume = 1.0f) override
	{
		if (m_SoundLibrary.find(key) != m_SoundLibrary.end())
		{
			auto sound = m_SoundLibrary.at(key);

			Mix_Volume(-1, int(volume * 128.0f));
			Mix_PlayChannel(-1, sound, 0);
		}
		else
		{
			//failed
		}
	};

	void Play(const MusicNames& key, float volume = 1.0f) override
	{
		if (m_MusicLibrary.find(key) != m_MusicLibrary.end())
		{
			//Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
			auto sound = m_MusicLibrary.at(key);
			
			Mix_VolumeMusic(int(volume * 128.0f));
			Mix_PlayMusic(sound, 1);
		}
		else
		{
			//failed
		}
	};

	void Start()
	{
		Mix_Resume(-1);
		Mix_ResumeMusic();
	}

	void Stop()
	{
		Mix_Pause(-1);
		Mix_PauseMusic();
	}


private:
	std::atomic_bool m_Playing;
	std::mutex m_Mutex;
	std::condition_variable m_QueueActive{};
	std::queue<SoundUnit> m_SoundQueue{};

	std::map<MusicNames, Mix_Music*> m_MusicLibrary;
	std::map<SoundNames, Mix_Chunk*> m_SoundLibrary;
};

