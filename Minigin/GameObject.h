#pragma once
#include "SceneObject.h"
#include "Components.h"

enum class Event
{
	ActorDied,
	ScoreSet,
	ColorChanged = 25,
	CoilyHitWithFlyingDisc = 500,
	DiscLeftAtEndOfTheStage = 50,
	CatchSlickOrSam = 300
};

class Observer;
class Subject;
class Component;
class Texture2D;

class GameObject final : public dae::SceneObject
{
public:

	template <typename T>
	T* GetComponent() const
	{
		for (auto component : m_pComponents)
		{
			if (dynamic_cast<T*>(component))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	void Update() override;
	void Render() const override;

	void Notify(Event event);

	void AddComponent(BaseComponent* pComponent);
	void AddWatcher(Observer* pObserver);

	GameObject(const std::string& name);
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	std::vector<BaseComponent*> m_pComponents;
	std::unique_ptr<Subject> m_spActorChanged;
};
