#pragma once
#include "Components.h"
#include "GameObject.h"
#include <glm\vec2.hpp>
#include "Scene.h"
#include <SDL.h>

class LevelComponent : public BaseComponent
{
public:
	LevelComponent(dae::Scene& scene, const glm::vec3& firstCubePos);

	glm::vec3 GetOffset() const { return m_Offset; }

private:
	void Initialize(dae::Scene& scene);
	void CreateMap(dae::Scene& scene);
	void CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene);

private:

	//const int m_MaxCubes = 28;

	const int m_FirstRowCubeCount;
	const glm::vec3 m_FirstCubePos;
	const int m_CubeColumnCount;
	const SDL_Rect m_CubeSrcRect;
	glm::vec3 m_Offset;
	const float m_CubeScale;

	std::shared_ptr<GameObject> m_Cubes[28];

};