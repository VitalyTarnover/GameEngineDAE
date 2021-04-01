#pragma once
class AudioService
{
public:
	enum class SoundNames
	{
		BoomEffect,
		ChargeEffect,
	};

	enum class MusicNames
	{
		Background01,
		Background02
	};

	struct SoundUnit
	{
		int id;
		float volume;
		bool music;
	};

	virtual ~AudioService() = default;

	AudioService() = default;
	AudioService(const AudioService&) = delete;
	AudioService& operator=(const AudioService&) = delete;
	AudioService(AudioService&&) = delete;
	AudioService& operator= (AudioService&&) = delete;
	
	virtual void QueueSound(const SoundNames&, float = 1.0f) = 0;
	virtual void QueueSound(const MusicNames&, float = 1.0f) = 0;
	
	virtual void AddSoundToLibrary(const SoundNames&, const std::string&) = 0;
	virtual void AddSoundToLibrary(const MusicNames&, const std::string&) = 0;

	virtual void Update() = 0;

	virtual void Pause() = 0;

protected:

	virtual void Play(const SoundNames&, float = 1.0f) = 0;
	virtual void Play(const MusicNames&, float = 1.0f) = 0;

	//virtual void StopAllSoundEffects() = 0;
	//virtual void StopSoundStream(SoundNames ) = 0;
	//virtual void StopAllSoundStreams() = 0;
};

