#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	//for (auto& scene : m_Scenes)
	//{
	//	scene->Update();
	//}
	m_pCurrentScene->Update();
}

void dae::SceneManager::Render()
{
	//for (const auto& scene : m_Scenes)
	//{
	//	scene->Render();
	//}
	m_pCurrentScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	m_pCurrentScene = scene;

	return *scene;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetSceneByName(const std::string& n) const
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName().compare(n) == 0)
			return scene;
	}
	return nullptr;
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}

void dae::SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& scene)
{
	m_pCurrentScene = scene;
}