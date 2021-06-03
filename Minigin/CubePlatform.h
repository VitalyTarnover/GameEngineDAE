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


	bool GetState2() const { return m_State2; }
	void SetState2(bool newState) { m_State2 = newState; }
	
	bool GetState3() const { return m_State3; }
	void SetState3(bool newState) { m_State3 = newState; }
	
	bool GetHasDiscNextToIt() const { return m_HasDiscNextToIt; }
	void SetHasDiscNextToIt(bool hasDisc) { m_HasDiscNextToIt = hasDisc; }


private:

	std::shared_ptr<GameObject> m_pGameObject = nullptr;

	GameLevel m_CurrentGameLevel = GameLevel::Level1;

	bool m_State1 = true;
	bool m_State2 = false;
	bool m_State3 = false;
	bool m_HasDiscNextToIt = false;
};
