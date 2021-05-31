#pragma once
#include "GameObject.h"

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void OnNotify(const GameObject* actor, Event event) = 0;
};
