#pragma once
#include "Texture2D.h"
#include "Transform.h"
#include "ResourceManager.h"
#include <glm\vec2.hpp>
//#include "Scene.h"

#include "Renderer.h"
#include "Font.h"
#include "SystemTime.h"
#include <SDL.h>

#include <SDL_ttf.h>

class GameObject;

class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	virtual void Update() {};
	virtual void Render() {};
	virtual void SetOwnerObject(GameObject* pGameObject);
protected:
	GameObject* m_pGameObject = nullptr;
	bool m_IsInitialized = false;
};

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(const glm::vec3& pos, const glm::vec2& scale = glm::vec2{ 1,1 });

	dae::Transform GetTransform() const;
	//void SetTransform(const glm::vec3& newTransform) { m_Transform.SetPosition(newTransform.x, newTransform.y, newTransform.z);  };
	void SetPosition(const glm::vec3& position) 
	{ 
		m_SpriteRect.x = (int)position.x;
		m_SpriteRect.y = (int)position.y;
		m_Transform.SetPosition(position.x, position.y, 0); 
	}

	void SetScale(const glm::vec2& scale)
	{
		m_SpriteRect.w = (int)scale.x;
		m_SpriteRect.h = (int)scale.y;
		m_Transform.SetScale(scale.x, scale.y);
	}

	const SDL_Rect& GetRect() { return m_SpriteRect; }

private:
	dae::Transform m_Transform;
	SDL_Rect m_SpriteRect;
};

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename, float scale = 1.f);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const { return m_spTexture2D; };
	void Render() override;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	glm::vec3 m_Position = {};
	float m_Scale;
	
};

//for frames of characters
enum class AnimStates//this can have different names
{
	//Qbert
	OnPlatformRightUp = 0,
	MidAirRightUp = 1,
	OnPlatformLeftUp = 2,
	MidAirLeftUp = 3,
	OnPlatformRightDown = 4,
	MidAirRightDown = 5,
	OnPlatformLeftDown = 6,
	MidAirLeftDown = 7,
	//cubes
	PinkFirst = 0,
	BlueSecond = 1,
	GreenThird = 2,
	//disc
	DiscState1 = 0,
	DiscState2 = 1,
	DiscState3 = 2,
	DiscState4 = 3,
	//Enemies
	EnemyOnPlatformRightUp = 0,
	EnemyMidAirRightUp = 1,
	EnemyOnPlatformLeftUp = 2,
	EnemyMidAirLeftUp = 3,
	EnemyOnPlatformRightDown = 4,
	EnemyMidAirRightDown = 5,
	EnemyOnPlatformLeftDown = 6,
	EnemyMidAirLeftDown = 7,
};



class SpriteAnimComponent final : public BaseComponent
{
public:
	SpriteAnimComponent(int columnsNr);

	SDL_Rect GetSrcRect();
	void SetAnimState(AnimStates state) 
	{ 
		if ((int)state <= m_ColumnsNr)
		m_CurrentState = state; 
		else
		{
			std::cout << "Error! Incorrect animation state!" << '\n';
		}
	};

	AnimStates GetCurrentAnimState() const { return m_CurrentState; };
	int GetColumnsNr() const { return m_ColumnsNr; }
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	AnimStates m_CurrentState = AnimStates::OnPlatformRightUp;
	int m_ColumnsNr;
};


class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color, bool isVisible = true);

	void SetPosition(const glm::vec3& pos);
	void SetIsVisible(bool isVisible, float setVisibleFalseTimer = 0);
	void UpdateText(const std::string& text);
	void Update() override;
	void Render() override;
private:
	std::string m_Text;
	std::shared_ptr<dae::Font> m_spFont;
	std::shared_ptr<dae::Texture2D> m_spTexture;
	glm::vec3 m_Position = {};
	SDL_Color m_Color = {};
	float m_SetVisibleFalseTimer = {};
	bool m_IsVisible = false;
	bool m_SetTimer = false;
};

class FPSTextComponent final : public BaseComponent
{
public:
	FPSTextComponent(const std::shared_ptr<dae::Font>& font);
	~FPSTextComponent();

	void Update() override;
	void Render() override;
private:

	std::string m_FPS = "0";
	TextComponent* m_pText = nullptr;
};

class HealthComponent : public BaseComponent
{
public:
	HealthComponent(const unsigned int& health);

	const unsigned int& GetLives() const;
	void SetLives(const unsigned int&);
	void Die();
	void Respawn();
	bool IsAlive() const;
private:
	unsigned int m_Lives;
	bool m_Dead;
};

class ScoreComponent : public BaseComponent
{
public:
	ScoreComponent(const unsigned int& score);

	const unsigned int& GetScore() const;
	void SetScore(const unsigned int&);
	void IncreaseScore(const int score);
private:
	unsigned int m_Score;
};



