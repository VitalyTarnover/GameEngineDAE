#include "MiniginPCH.h"
#include "LivesObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Components.h"

void LivesObserver::OnNotify(const GameObject* actor, Event event)
{
	if (event == Event::ActorDied)
	{
		UpdateLives(actor);
	}
}

void LivesObserver::UpdateLives(const GameObject* actor)
{
	std::shared_ptr<dae::SceneObject> livesDisplay = nullptr;

	if (actor->GetName() == "Q*Bert") livesDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplay");
	else if (actor->GetName() == "Q*Bert2") livesDisplay = dae::SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("LivesDisplay2");

	auto textComp = std::static_pointer_cast<GameObject>(livesDisplay).get()->GetComponent<TextComponent>();

	auto healthComp = actor->GetComponent<HealthComponent>();

	textComp->UpdateText("Lives: " + std::to_string(healthComp->GetLives()));



}