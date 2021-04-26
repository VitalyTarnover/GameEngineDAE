#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "Renderer.h"



LevelComponent::LevelComponent(dae::Scene& scene)
    :m_FirstRowCubeCount(7)
    , m_CubeColumnCount(7)
    , m_CubeSrcRect{ 0,160,32,32 }
    , m_CubeOffset{ 16,24, 0}
    , m_HighestCubePos{ 640, 100, 0 }
    , m_CubeScale{3.0f}
{
    m_CubeOffset.x *= m_CubeScale;
    m_CubeOffset.y *= m_CubeScale;
    Initialize(scene);
}

void LevelComponent::Initialize(dae::Scene& scene)
{
    CreateMap(scene);
}


void LevelComponent::CreateMap(dae::Scene& scene)
{
    //16x24
    int rowCubeCount = m_FirstRowCubeCount;
    glm::vec3 highestCubePos = m_HighestCubePos;

    for (size_t j = 0; j < m_CubeColumnCount; j++)
    {
        for (size_t i = 0; i < rowCubeCount; i++)
        {
            glm::vec3 pos = highestCubePos;
            pos.x += m_CubeOffset.x * i;
            pos.y += m_CubeOffset.y * i;
			pos.z = 0;
            CreateCube(i, pos, scene);
        }
         highestCubePos.x -= m_CubeOffset.x;
         highestCubePos.y += m_CubeOffset.y;

         rowCubeCount--;
    }
}

void LevelComponent::CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene)
{
    auto cube = std::make_shared<GameObject>("Cube" + std::to_string(index));
    cube->AddComponent(new TransformComponent(pos));
    cube->AddComponent(new Texture2DComponent("CubePlatform1.png", m_CubeScale));
    m_Cubes[index] = cube;

    scene.Add(cube);
}
