#pragma once
#include "Observer.h"
#include "GameObject.h"

class ScoreObserver final : public Observer
{
public:
	void OnNotify(const GameObject* actor, Event event) override;

private:
	void UpdateScore(const GameObject* actor);
};
