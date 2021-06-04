#include "MiniginPCH.h"
#include "LevelComponent.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MovementComponent.h"
#include "LevelFileReader.h"
#include "CollisionCheckManager.h"

//Static variables Init
const int m_MaxCubes = 28;
const int m_SideLength = 7;

LevelComponent::LevelComponent(dae::Scene& scene, const glm::vec3& firstCubePos, float scale)
    :m_FirstRowCubeCount(7)
    , m_CubeColumnCount(7)
    , m_CubeSrcRect{ 0,160,32,32 }
    , m_Offset{ 16, 24, 0 }//???
    , m_HighestCubePos{ firstCubePos }
    , m_Scale{ scale }
{
    int mostLeftBlockIndex = 0;
    int lowestBlockIndex = 6;
    for (int i = 0; i < m_SideLength; i++)
    {
        m_MostLeftBlocks[i] = mostLeftBlockIndex;
        mostLeftBlockIndex += m_SideLength - i;

        m_MostRightBlocks[i] = i;

        m_LowestBlocks[i] = lowestBlockIndex; 
        lowestBlockIndex += 6 - i;
    }

    m_Offset.x *= m_Scale;
    m_Offset.y *= m_Scale;
    Initialize(scene);
}




void LevelComponent::Initialize(dae::Scene& scene)
{
    CreateMap(scene);
}



void LevelComponent::CreateMap(dae::Scene& scene)
{
    
    LevelFileReader LFR("Resources/Level.txt");
    std::vector<glm::vec3>cubePositions = LFR.ReadGetPositions();


    for (size_t i = 0; i < cubePositions.size(); i++)
    {
        CreateCube(i, glm::vec3(cubePositions[i].x * m_Scale + m_HighestCubePos.x, cubePositions[i].y * m_Scale + m_HighestCubePos.y, 0), scene);
    }
    
    SpawnDiscs();
    
}


void LevelComponent::SpawnDiscs()
{
    int index = 0;
    int row = 7;
    std::vector<int> leftSideIndices;
    std::vector<int> rightSideIndices;
    for (int i = 0; i < 7; i++)
    {
        index += i;

        leftSideIndices.push_back(i * row + index);

        if (i != 0) rightSideIndices.push_back(i);

        row--;
    }

    int leftSideCubeIndex = leftSideIndices[rand() % 5 + 2];
    int rightSideCubeIndex = rightSideIndices[rand() % 4 + 2];

    glm::vec3 disc1Pos = m_Cubes[leftSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();

    glm::vec3 disc2Pos = m_Cubes[rightSideCubeIndex]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition();


    disc1Pos.x -= m_Offset.x * 1.5f;
    disc2Pos.x += m_Offset.x * 2.5f;

    for (auto& cube : m_Cubes) cube->SetHasDiscNextToIt(false);

    for (auto& disc : m_Discs) disc->SetIsUsed(true);//

    

    CreateDisc(disc1Pos, *dae::SceneManager::GetInstance().GetCurrentScene());
    m_Cubes[leftSideCubeIndex]->SetHasDiscNextToIt(true);
    CreateDisc(disc2Pos, *dae::SceneManager::GetInstance().GetCurrentScene());
    m_Cubes[rightSideCubeIndex]->SetHasDiscNextToIt(true);

}


void LevelComponent::Update()
{
    for (auto& cube : m_Cubes)
    {
        if (cube)
            cube->Update();
    }

    for (auto& disc : m_Discs)
    {
        if (disc)
            disc->Update();
    }


    LevelCompletedCheck();
    DeleteUsedDiscs();

}

void LevelComponent::Render()
{
    for (const auto& cube : m_Cubes)
    {
        if (cube)
            cube->Render();
    }
}

bool LevelComponent::GetNextCubeIndex(int& currentIndex, const AnimStates& dir) const//needs reworking
{
    int columnIndex = GetColumnNumber(currentIndex);

    switch (dir)
    {
    case AnimStates::MidAirRightUp:
    {
        for (size_t i = 0; i < m_SideLength; i++)
            if (currentIndex == m_MostRightBlocks[i])
                return false;

        int columnIndexAfterJump = columnIndex - 1;
        currentIndex -= m_FirstRowCubeCount - columnIndexAfterJump;

        break;
    }
    case AnimStates::MidAirLeftUp:
    {
        for (size_t i = 0; i < m_SideLength; i++)
            if (currentIndex == m_MostLeftBlocks[i])
                return false;

        currentIndex--;

        break;
    }
    case AnimStates::MidAirRightDown:
    {
        for (size_t i = 0; i < m_SideLength; i++)
            if (currentIndex == m_LowestBlocks[i])
                return false;

        currentIndex++;

        break;
    }
    case AnimStates::MidAirLeftDown:
    {
        for (size_t i = 0; i < m_SideLength; i++)
            if (currentIndex == m_LowestBlocks[i])
                return false;

        currentIndex += m_FirstRowCubeCount - columnIndex;

        break;
    }
    }

    return true;
}

void LevelComponent::SwitchGameLevel(GameLevel gameLevel)
{
    for (auto& cube : m_Cubes)
    {
        cube->ResetCubeState();
        cube->ChangeGameLevel(gameLevel);
    }

    SpawnDiscs();
}

void LevelComponent::LevelCompletedCheck()
{
    auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();
    
    if (currentScene->GetGameLevel() == GameLevel::Level1)
    {
        for (auto& cube : m_Cubes)
        {
            if (!cube->GetState2())
                return;
        }

        TeleportPlayersToSpawnPos();

        currentScene->SetGameLevel(GameLevel(int(currentScene->GetGameLevel()) + 1));

        SwitchGameLevel(currentScene->GetGameLevel());
    }
    else if (currentScene->GetGameLevel() == GameLevel::Level2)
    {
        for (auto& cube : m_Cubes)
        {
            if (!cube->GetState3())
                return;
        }

        TeleportPlayersToSpawnPos();

        currentScene->SetGameLevel(GameLevel(int(currentScene->GetGameLevel()) + 1));

        SwitchGameLevel(currentScene->GetGameLevel());
    }
    else if (currentScene->GetGameLevel() == GameLevel::Level3)
    {
        for (auto& cube : m_Cubes)
        {
            if (!cube->GetState2())
                return;
        }

        currentScene->GetPlayer(0);

        
        TeleportPlayersToSpawnPos();

        currentScene->SetGameLevel(GameLevel(0));

        SwitchGameLevel(currentScene->GetGameLevel());
    }
    

}


void LevelComponent::TeleportPlayersToSpawnPos()
{
   
    //gamemodes switch here
    auto cube = GetCube(0);

    auto currentScene = dae::SceneManager::GetInstance().GetCurrentScene();
    auto player1 = currentScene->GetPlayer(0);

    glm::vec3 startingPosition{ 0,0,0 };
    startingPosition.x = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().x + (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 8.f);
    startingPosition.y = cube->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition().y - (dae::SceneManager::GetInstance().GetCurrentScene()->GetSceneScale() * 10.f);

    player1->GetComponent<TransformComponent>()->SetPosition(startingPosition);
    player1->GetComponent<MovementComponent>()->SetCurrentCubeIndex(0);
    //player 2 too
}


int LevelComponent::GetColumnNumber(const int& currentTileIndex) const
{
    int cubeCount = m_FirstRowCubeCount;

    for (int i = 0; i < m_CubeColumnCount; i++)
    {
        if (currentTileIndex < cubeCount)
            return i;

        cubeCount += (m_FirstRowCubeCount - (i + 1));
    }

    return -1;
}

void LevelComponent::CreateCube(const size_t& index, const glm::vec3& pos, dae::Scene& scene)
{

    m_Cubes[index] = std::make_shared<CubePlatform>();
    auto newCube = m_Cubes[index]->GetGameObject();
    newCube->AddComponent(new TransformComponent(pos));
    newCube->AddComponent(new Texture2DComponent("CubePlatforms.png", m_Scale));
    newCube->AddComponent(new SpriteAnimComponent(3));
    scene.Add(newCube);

}

void LevelComponent::CreateDisc(const glm::vec3& pos, dae::Scene& scene)
{
    m_Discs.push_back(std::make_shared<DiscPlatform>(m_Cubes[0]->GetGameObject()->GetComponent<TransformComponent>()->GetTransform().GetPosition()));
    auto newDisc = m_Discs[m_Discs.size() - 1]->GetGameObject();
    
    newDisc->AddComponent(new TransformComponent(pos, glm::vec2{ 18,8 }));
    newDisc->AddComponent(new Texture2DComponent("disc.png", m_Scale));
    newDisc->AddComponent(new SpriteAnimComponent(4));
    CollisionCheckManager::GetInstance().AddObjectForCheck(newDisc);
    scene.Add(newDisc);
}

void LevelComponent::DeleteUsedDiscs()
{
    if (m_Discs.size() > 0)
    {
        for (size_t i = 0; i < m_Discs.size(); i++)
        {
            if (m_Discs[i])
            {
                if (m_Discs[i]->GetIsUsed())
                {
                    dae::SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(m_Discs[i]->GetGameObject());
                    CollisionCheckManager::GetInstance().DeleteGameObject(m_Discs[i]->GetGameObject());
                    m_Discs.erase(m_Discs.begin() + i);
                }
            }
        }
    }
    

}

std::shared_ptr<DiscPlatform> LevelComponent::GetDisc(std::shared_ptr<GameObject> gameObject)
{
    return *std::find_if(
        m_Discs.begin(), m_Discs.end(),
        [&](const std::shared_ptr<DiscPlatform>& x) 
        { 
             return x->GetGameObject() == gameObject; 
        });
}

void LevelComponent::DeleteDisc(std::shared_ptr<DiscPlatform> discToDelte)
{
    m_Discs.erase(std::remove(m_Discs.begin(), m_Discs.end(), discToDelte), m_Discs.end());
    dae::SceneManager::GetInstance().GetCurrentScene()->DeleteGameObject(discToDelte->GetGameObject());

}
