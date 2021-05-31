#pragma once
#include "AudioService.h"

class AudioNullService final : public AudioService
{
public:
	AudioNullService() = default;
	~AudioNullService() override = default;
	
	void QueueSound(const SoundNames&, float = 1.0f) override {};
	void QueueSound(const MusicNames&, float = 1.0f) override {};
	
	void Play(const SoundNames&, float = 1.0f) override {};
	void Play(const MusicNames&, float = 1.0f) override {};
	
	void Pause() override {};
	
	void Update() override {};
	
	void AddSoundToLibrary(const SoundNames&, const std::string&) override {};
	void AddSoundToLibrary(const MusicNames&, const std::string&) override {};
};

