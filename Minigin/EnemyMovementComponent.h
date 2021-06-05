#pragma once
#include "GameObject.h"
#include "BaseMovementComponent.h"

class EnemyMovementComponent final : public BaseMovementComponent
{
public:

    enum class EnemyType
    {
        Coily,
        Sam,
        Slick,
        Ugg,
        WrongWay
    };

    EnemyMovementComponent(const std::shared_ptr<GameObject>& pPlayer, EnemyType enemyType, bool startOnLeftSide = false);
    void Update();

private:
    //Methods
    void FollowPlayer();
    void Descend();
    void SidewaysMovement();
    void SidewaysJump();
    void SidewaysFall();

    //Datamembers
    EnemyType m_EnemyType;
    std::shared_ptr<GameObject> m_pPlayer;
    float m_CurrentJumpTime;
    const float m_MaxJumpTime;
    bool  m_StartOnLeftSide;
};