#pragma once
#include "SceneManager.h"
#include "GameObject.h"


enum class GameLevel//different games can have different amount of levels
{
	Level1,
	Level2,
	Level3
};

enum class GameMode
{
	MainMenu,
	SinglePlayer,
	Coop,
	Versus
};


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

		void SetGameLevel(GameLevel levelNumber);

		GameLevel GetGameLevel() const { return m_CurrentGameLevel; }

		std::shared_ptr<SceneObject> GetObjectByName(const std::string& name) const;
		void AddPlayer(const std::shared_ptr<GameObject>& player);
		std::shared_ptr<GameObject> GetPlayer(int index);


		void AddLevel(const std::shared_ptr<GameObject>& level)
		{
			m_pLevels.push_back(level);
			m_pCurrentLevel = level;
		}
		std::shared_ptr<GameObject> GetLevel(int index) const {return m_pLevels[index];}
		
		void SetCurrentLevel(const std::shared_ptr<GameObject>& level) { m_pCurrentLevel = level; }
		std::shared_ptr<GameObject> GetCurrentLevel() const { return m_pCurrentLevel; };

		void DeleteGameObject(std::shared_ptr<SceneObject> objectToDelete);
		
		void DeleteMarkedForDelteGameObjects() 
		{
			for (size_t i = 0; i < m_Objects.size(); i++)
			{
				if (m_Objects[i]->GetMarkedForDelete())
				{
					//m_Objects.erase(m_Objects.begin() + i);
					m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), m_Objects[i]), m_Objects.end());
					//--i;
				}
			}
		}

		void ClearScene()
		{
			m_Objects.clear();
			m_pPlayers.clear();
			m_pLevels.clear();
			m_pCurrentLevel = nullptr;
		}

		GameMode GetCurrentGameMode() const { return m_CurrentGameMode; }
		void SetCurrentGameMode(GameMode gameMode) { m_CurrentGameMode = gameMode; }

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<SceneObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_pPlayers{};

		glm::vec3 m_PlayerSpawnPos;

		float m_SceneScale = 3.0f;
		
		std::vector< std::shared_ptr<GameObject>> m_pLevels;
		std::shared_ptr<GameObject> m_pCurrentLevel;

		static unsigned int m_IdCounter;

		GameLevel m_CurrentGameLevel = GameLevel::Level1;
		GameMode m_CurrentGameMode = GameMode::SinglePlayer;
	};
}
