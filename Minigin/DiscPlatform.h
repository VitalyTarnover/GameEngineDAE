#pragma once
#include "GameObject.h"
#include "Scene.h"


class DiscPlatform
{
public:
	DiscPlatform(const glm::vec3& finalPos);

	DiscPlatform(const DiscPlatform& other) = delete;
	DiscPlatform(DiscPlatform&& other) = delete;
	DiscPlatform& operator=(const DiscPlatform& other) = delete;
	DiscPlatform& operator=(DiscPlatform&& other) = delete;
	~DiscPlatform() = default;

	void Update();
	void Render() const;

	std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject; };

	void SetIsMovingToTop(bool isMoving, int playerIndex)
	{
		m_IsMovingToTop = isMoving;
		m_PlayerIndex = playerIndex;
	}

	bool GetIsUsed() const { return m_IsUsed; }
	void SetIsUsed(bool isUsed) { m_IsUsed = isUsed; }

private:
	void SwitchColors();
	void MoveToTheTop();

	std::shared_ptr<GameObject> m_pGameObject = nullptr;
	TransformComponent* m_pTransformComponent = nullptr;
	
	float m_AnimTimer = 0;
	float m_AnimTime = 0.2f;

	float m_MoveFactor = 0.f;
	float m_Speed = 200.f;


	glm::vec3 m_FinalPos;
	glm::vec3 m_Direction = glm::vec3{ 0,0,0 };

	bool m_IsMovingToTop = false;
	bool m_IsUsed = false;
	int m_PlayerIndex = 0;
};

