#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
//#include "LevelComponent.h"
//#include "MovementComponent.h"

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

void Scene::DeleteGameObject(std::shared_ptr<SceneObject> objectToDelete)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] == objectToDelete)
		{
			
			//m_Objects.erase(m_Objects.begin() + i);
			m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), m_Objects[i]), m_Objects.end());

		}
	}
}

void Scene::Update()
{

	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]) m_Objects[i]->Update();
	}

	DeleteMarkedForDelteGameObjects();

}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (object) object->Render();
	}
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

void dae::Scene::SetGameLevel(GameLevel levelNumber)
{
	m_CurrentGameLevel = levelNumber;
	//m_pCurrentLevel->GetComponent<LevelComponent>()->SwitchGameLevel(levelNumber);
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
	m_PlayerSpawnPos = player->GetComponent<TransformComponent>()->GetTransform().GetPosition();
}


std::shared_ptr<GameObject> Scene::GetPlayer(int index)
{
	if (m_pPlayers.size() <= (unsigned)index)
	{
		return nullptr;
	}

	return m_pPlayers[index];
}

