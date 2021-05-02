#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<SceneObject>& object);

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const;
		void Update();
		void Render() const;

		
		float GetSceneScale() const { return m_SceneScale; }

		void SetCurrentLevel(const std::shared_ptr<GameObject>& level) { m_pCurrentLevel = level; }
		std::shared_ptr<GameObject> GetCurrentLevel() const { return m_pCurrentLevel; }
		void AddLevel(const std::shared_ptr<GameObject>& level)
		{
			m_pLevels.push_back(level);
			m_pCurrentLevel = level;
		}

		std::shared_ptr<GameObject> GetLevel(int index) const { return m_pLevels[index]; }

		std::shared_ptr<SceneObject> GetObjectByName(const std::string& name) const;
		void AddPlayer(const std::shared_ptr<GameObject>& player);
		std::shared_ptr<GameObject> GetPlayer(int index);

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<SceneObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_pPlayers{};

		float m_SceneScale = 1.0f;
		
		std::vector< std::shared_ptr<GameObject>> m_pLevels;
		std::shared_ptr<GameObject> m_pCurrentLevel;

		static unsigned int m_IdCounter;

	};
}
