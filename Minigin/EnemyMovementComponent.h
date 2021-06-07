#pragma once
#include "GameObject.h"
#include "BaseMovementComponent.h"

class EnemyMovementComponent final : public BaseMovementComponent
{
public:

    enum class EnemyType
    {
        Coily,
        WrongWay,
        Ugg,
        Sam,
        Slick,
    };

    EnemyMovementComponent(const std::shared_ptr<GameObject>& pPlayer, EnemyType enemyType, float speed = 100.f, float jumpTime = 1.25f);
    void Update();

private:
    void FollowPlayer();
    void Descend();
    void SidewaysMovement();
    void SidewaysJump();
    void SidewaysFallToDeath();

    EnemyType m_EnemyType;
    std::shared_ptr<GameObject> m_pPlayer;
    float m_JumpTimer;
    const float m_JumpTime;
};