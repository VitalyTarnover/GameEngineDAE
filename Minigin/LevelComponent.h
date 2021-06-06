#pragma once
#include "Components.h"
#include "GameObject.h"
#include <glm\vec2.hpp>
#include "Scene.h"
#include <SDL.h>
#include "CubePlatform.h"
#include "DiscPlatform.h"
#include <vector>

class LevelComponent : public BaseComponent
{
public:
	LevelComponent(dae::Scene& scene, const glm::vec3& firstCubePos, float scale);

	const glm::vec3& GetOffset() const { return m_Offset; }

	void Update() override;

	void Render() override;

	std::shared_ptr<CubePlatform> GetCube(int index) const { return m_Cubes[index]; }

	bool JumpToNextCube(int& currentIndex, AnimStates dir, bool isSidewaysJump, int currentColumn, int currentRow) const; //False if Qbert jumps off the map



	void TeleportPlayersToSpawnPos();

	std::shared_ptr<DiscPlatform> GetDisc(int index) { return m_Discs[index];}

	std::shared_ptr<DiscPlatform> GetDisc(std::shared_ptr<GameObject> gameObject);

	void DeleteDisc(std::shared_ptr<DiscPlatform>);

private:
	void Initialize(dae::Scene& scene);
	void CreateMap(dae::Scene& scene);
	void CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene);
	void CreateDisc(const glm::vec3& pos, dae::Scene& scene);
	void DeleteUsedDiscs();
	int GetRowNumber(const int& currentTileIndex) const;
	void LevelCompletedCheck();
	void SwitchGameLevel(GameLevel gameLevel);
	void SpawnDiscs();

	void FlashCubesOnCompletion();



	const int m_FirstRowCubeCount;
	const int m_CubeColumnCount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec3 m_Offset;
	const glm::vec2 m_HighestCubePos;

	float m_Scale;

	bool m_FlashingCubes = false;
	float m_EntireFlashCubesTimer = 0.f;
	float m_EntireFlashCubesTime = 1.5f;

	const static int m_SideLength = 7;
	int m_MostLeftBlocks[m_SideLength];
	int m_MostRightBlocks[m_SideLength];
	int m_LowestBlocks[m_SideLength];
	
	const static int m_MaxCubes = 28;
	std::shared_ptr<CubePlatform> m_Cubes[m_MaxCubes];

	std::vector<std::shared_ptr<DiscPlatform>> m_Discs;

};