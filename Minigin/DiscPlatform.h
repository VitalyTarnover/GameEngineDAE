#pragma once
#include "GameObject.h"
#include "Scene.h"


class DiscPlatform
{
public:
	DiscPlatform();

	DiscPlatform(const DiscPlatform& other) = delete;
	DiscPlatform(DiscPlatform&& other) = delete;
	DiscPlatform& operator=(const DiscPlatform& other) = delete;
	DiscPlatform& operator=(DiscPlatform&& other) = delete;
	~DiscPlatform() = default;

	void Update();
	void Render() const;

	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };

private:
	void SwitchColors();

	std::shared_ptr<GameObject> m_pGameObject = nullptr;
	float m_AnimTimer = 0;
	float m_AnimTime = 0.5f;

};

