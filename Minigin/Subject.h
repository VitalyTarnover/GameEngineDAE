#pragma once
#include "GameObject.h"
#include "Observer.h"

class Subject
{
	friend class GameObject;

public:
	Subject();
	~Subject();
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

protected:
	void Notify(const GameObject* actor, Event event);

private:
	const static int m_MaxObserverCount = 3;
	unsigned int m_ObserverCount;

	Observer* m_pObservers[m_MaxObserverCount];
};
