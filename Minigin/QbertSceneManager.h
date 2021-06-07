#pragma once
#include "Singleton.h"


class QbertSceneManager final : public dae::Singleton<QbertSceneManager>
{
public:
	void LoadMainMenu();
	void LoadSinglePlayer();
	void LoadCoop();
	void LoadVersus();

private:
	friend class dae::Singleton<QbertSceneManager>;
};

