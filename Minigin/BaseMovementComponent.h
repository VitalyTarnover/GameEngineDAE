#pragma once
#include "Components.h"
#include "GameObject.h"

class BaseMovementComponent : public BaseComponent
{
public:
    explicit BaseMovementComponent();
    virtual void Update() = 0;
    void SetCurrentCubeIndex(int index) { m_CurrentCubeIndex = index; };

    void SetMovementLocked(bool lock) { m_MovementLocked = lock; }
    bool GetMovementLocked() const { return m_MovementLocked; }

    bool GetIsInDeathZone() const { return m_IsInDeathZone; }
    bool GetIsFallingToDeath() const { return m_FallingToDeath;  }

protected:

    bool GetIsFallingToDeathBehindMap() const { return m_FallingToDeath && !m_FirstHalfOfTheJump && (m_Direction == AnimStates::MidAirLeftUp || m_Direction == AnimStates::MidAirRightUp); };
    void ActivateJump(bool isSideWaysJump = false);
    void FallToDeath();
    void Jump();
    
    float m_FallToDeathTimer = 0.f;
    float m_FallToDeathTime = 0.5f;

    bool m_MovementLocked = false;
    bool m_IsMoving;
    bool m_FallingToDeath = false;
    bool m_IsInDeathZone = false;
    float m_Speed;
    glm::vec2 m_MoveDistance;
    bool m_FirstHalfOfTheJump;
    AnimStates m_Direction;
    glm::vec2 m_JumpStartPos;
    int m_CurrentCubeIndex;
    int m_SourceHeightOffset;
    int m_CurrentColumn;
    int m_CurrentRow;
};