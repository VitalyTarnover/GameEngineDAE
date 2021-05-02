#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) 
	: m_Name(name)
	, m_pCurrentLevel{}
	, m_pLevels{}
	, m_Objects{}
	, m_pPlayers{}
{}

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update()
{
	for (const auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

std::shared_ptr<SceneObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_Objects)
	{
		if (object->GetName().compare(name) == 0)
		{
			return object;
		}
	}
	return nullptr;
}

void Scene::AddPlayer(const std::shared_ptr<GameObject>& player)
{
	m_pPlayers.push_back(player);
}

std::shared_ptr<GameObject> Scene::GetPlayer(int index)
{
	if (m_pPlayers.size() <= (unsigned)index)
	{
		return nullptr;
	}

	return m_pPlayers[index];
}