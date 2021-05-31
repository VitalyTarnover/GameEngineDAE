#pragma once
#include "AudioService.h"
#include "AudioNullService.h"



class AudioLocator final
{
public:
	//static void Initialize() { m_Service = &m_NullService; }
	
	static void SetAudioService(AudioService* ss){ m_Service = ss == nullptr ? &m_NullService : ss; }

	//static void Provide(AudioService* service)
	//{
	//	if (service == NULL)
	//	{
	//		// Revert to null service.
	//		m_Service = &m_NullService;
	//	}
	//	else
	//	{
	//		m_Service = service;
	//	}
	//}
	
	static AudioService& GetAudioService() { return *m_Service; }


	~AudioLocator() { if (m_Service) delete m_Service; }

	static void Reset() { if (m_Service) delete m_Service; }



private:
	static AudioService* m_Service;
	static AudioNullService m_NullService;

};

AudioService* AudioLocator::m_Service{ nullptr };
AudioNullService AudioLocator::m_NullService;