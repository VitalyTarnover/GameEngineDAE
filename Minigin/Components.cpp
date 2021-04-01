#include "MiniginPCH.h"
#include "Components.h"
#include "GameObject.h"

void BaseComponent::SetOwnerObject(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
}

TransformComponent::TransformComponent(const glm::vec3& pos)
{
	m_Transform.SetPosition(pos.x, pos.y, pos.z);
}

dae::Transform TransformComponent::GetTransform() const
{
	return m_Transform;
}


Texture2DComponent::Texture2DComponent(const std::string& filename)
	:m_spTexture2D(dae::ResourceManager::GetInstance().LoadTexture(filename))
{
}



void Texture2DComponent::Render()
{
	if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>())// do once
	{
		m_IsInitialized = true;
		m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	}
	
	dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, m_Position.x, m_Position.y);
};

TextComponent::TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& font, const SDL_Color& color, bool isVisible)
	:m_Text{ text }
	, m_spFont{ font }
	, m_Color{ color }
	, m_IsVisible{ isVisible }
{
	const auto surf = TTF_RenderText_Blended(m_spFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_spTexture = std::make_shared<dae::Texture2D>(texture);
}

void TextComponent::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
}

void TextComponent::SetIsVisible(bool isVisible, float setVisibleFalseTimer)
{
	m_IsVisible = isVisible;


	if (setVisibleFalseTimer != 0)
	{
		m_SetTimer = true;
		m_SetVisibleFalseTimer = setVisibleFalseTimer;
	}
}

void TextComponent::UpdateText(const std::string& text)
{
	m_Text = text;
}

void TextComponent::Update()
{
	if (m_SetTimer)
	{
		m_SetVisibleFalseTimer -= Time::GetInstance().GetDeltaTime();

		if (m_SetVisibleFalseTimer <= 0.0f)
		{
			m_SetTimer = false;
			m_SetVisibleFalseTimer = 0.0f;
			m_IsVisible = false;
		}
	}
}

void TextComponent::Render()
{
	if (!m_IsInitialized && m_pGameObject && m_pGameObject->GetComponent<TransformComponent>()) // doonce
	{
		m_IsInitialized = true;
		m_Position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	}
	
	if (m_spTexture && m_IsVisible)
	{
		const auto surf = TTF_RenderText_Blended(m_spFont->GetFont(), m_Text.c_str(), m_Color);
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_FreeSurface(surf);
		m_spTexture = std::make_shared<dae::Texture2D>(texture);
		dae::Renderer::GetInstance().RenderTexture(*m_spTexture, m_Position.x, m_Position.y);
	}
};


FPSTextComponent::FPSTextComponent(const std::shared_ptr<dae::Font>& font)
	: m_pText{ new TextComponent(m_FPS, font , SDL_Color{ 255,255,0 }) }
{
}

FPSTextComponent::~FPSTextComponent()
{
	delete m_pText;
}

void FPSTextComponent::Update()
{
	if (!m_IsInitialized && m_pGameObject->GetComponent<TransformComponent>()) // doonce
	{
		m_IsInitialized = true;
		m_pText->SetPosition(m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition());
	}

	m_FPS = "FPS: " + std::to_string(int(1.f / Time::GetInstance().GetDeltaTime()));
	m_pText->UpdateText(m_FPS);
};

void FPSTextComponent::Render()
{
	m_pText->Render();
}

HealthComponent::HealthComponent(const unsigned int& health)
	: m_Lives{ health }
	, m_Dead{ false }
{
}

void HealthComponent::SetLives(const unsigned int& health)
{
	m_Lives = health;
}

void HealthComponent::Die()
{
	m_Dead = true;

	if ((int)m_Lives - 1 < 0)
		return;

	m_Lives--;
	m_pGameObject->Notify(Event::ActorDied);
}

void HealthComponent::Respawn()
{
	m_Dead = false;
}

const unsigned int& HealthComponent::GetLives() const
{
	return m_Lives;
}

bool HealthComponent::IsAlive() const
{
	return (!m_Dead);
}

ScoreComponent::ScoreComponent(const unsigned int& score)
	: m_Score{ score }
{
}

void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify(Event::ScoreSet);
}

void ScoreComponent::IncreaseScore(const int score)
{
	m_Score += score;
	m_pGameObject->Notify(Event(score));
}

const unsigned int& ScoreComponent::GetScore() const
{
	return m_Score;
}

