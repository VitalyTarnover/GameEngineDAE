#pragma once
#include "Observer.h"
#include "GameObject.h"

class LivesObserver final : public Observer
{
public:
	void OnNotify(const GameObject* actor, Event event) override;

private:
	void UpdateLives(const GameObject* actor);
};
