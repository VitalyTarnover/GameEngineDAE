#pragma once
#include "GameObject.h"
#include "CubePlatform.h"

class Playfield
{
public:
	void Initialize();

private:
	GameObject m_GameObject;
	CubePlatform m_CubePlatformArray[28];//make a proper macros or static
};

