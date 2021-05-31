#pragma once
#include "GameObject.h"
#include "Scene.h"

class CubePlatform
{
public:
	CubePlatform();

	CubePlatform(const CubePlatform& other) = delete;
	CubePlatform(CubePlatform&& other) = delete;
	CubePlatform& operator=(const CubePlatform& other) = delete;
	CubePlatform& operator=(CubePlatform&& other) = delete;
	~CubePlatform() = default;

	void Update();
	void Render() const;

	void ResetCubeState()
	{
		m_State1 = true;
		m_State2 = false;
		m_State3 = false;
		m_pGameObject->GetComponent<SpriteAnimComponent>()->SetAnimState(AnimStates::PinkFirst);
	}

	void SwitchColor(const std::string& playerName);
	void ChangeGameLevel(GameLevel gameLevel) 
	{ m_CurrentGameLevel = gameLevel; }
	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };

	bool m_State2 = false;
	bool m_State3 = false;
private:

	std::shared_ptr<GameObject> m_pGameObject = nullptr;

	GameLevel m_CurrentGameLevel = GameLevel::Level1;

	bool m_State1 = true;
};
