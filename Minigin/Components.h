#pragma once
#include "Texture2D.h"
#include "Transform.h"
#include "ResourceManager.h"

#include "Renderer.h"
#include "Font.h"
#include "Time.h"
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
	TransformComponent(const glm::vec3& pos);

	dae::Transform GetTransform() const;
private:
	dae::Transform m_Transform;
};

class Texture2DComponent final : public BaseComponent
{
public:
	Texture2DComponent(const std::string& filename);

	std::shared_ptr<dae::Texture2D> GetTexture2D() const { return m_spTexture2D; };
	void Render() override;
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
	glm::vec3 m_Position = {};
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
