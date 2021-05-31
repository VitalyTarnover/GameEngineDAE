#pragma once
#include "Components.h"
#include "GameObject.h"
#include <glm\vec2.hpp>
#include "Scene.h"
#include <SDL.h>
#include "CubePlatform.h"

class LevelComponent : public BaseComponent
{
public:
	LevelComponent(dae::Scene& scene, const glm::vec3& firstCubePos, float scale);

	const glm::vec3& GetOffset() const { return m_Offset; }

	void Update() override;

	void Render() override;

	std::shared_ptr<CubePlatform> GetCube(int index) { return m_Cubes[index]; }

	bool GetNextCubeIndex(int& currentIndex, const AnimStates& dir) const; //False if Qbert jumps off the map

	void SwitchGameLevel(GameLevel gameLevel);

	void LevelCompletedCheck();

	void TeleportPlayersToSpawnPos();

private:
	void Initialize(dae::Scene& scene);
	void CreateMap(dae::Scene& scene);
	void CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene);

	int GetColumnNumber(const int& currentTileIndex) const;

	const static int m_MaxCubes = 28;

	const int m_FirstRowCubeCount;
	const int m_CubeColumnCount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec3 m_Offset;
	const glm::vec2 m_HighestCubePos;

	float m_Scale;

	const static int m_SideLength = 7;
	int m_MostLeftBlocks[m_SideLength];
	int m_MostRightBlocks[m_SideLength];
	int m_LowestBlocks[m_SideLength];
	std::shared_ptr<CubePlatform> m_Cubes[m_MaxCubes];

};