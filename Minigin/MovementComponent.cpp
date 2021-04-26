#include "MiniginPCH.h"
#include "MovementComponent.h"
#include "GameObject.h"



void MovementComponent::Move(MoveDirections moveDir)
{
	m_CurrentPos = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	
	m_DesiredPos = m_CurrentPos;
	m_DesiredPos.x += 50;
	m_DesiredPos.y -= 150;
	
	m_MoveDirection = moveDir;
	m_Jumped = true;
}

void MovementComponent::Update()
{
	if (m_Jumped)
	{
		switch (m_MoveDirection)
		{
		case MoveDirections::MoveUpRight:

			m_pGameObject->GetComponent<TransformComponent>()->SetTransform(m_CurrentPos);

			if (m_CurrentPos.y >= m_DesiredPos.y - 50)
			{
				m_CurrentPos.y -= 2;
			}
			else
			{
				m_CurrentPos.y += 2;
			}
			m_CurrentPos.x++;

			//increase time for when to go down
			m_JumpTime += Time::GetInstance().GetDeltaTime();
			//stop jump
			if (m_CurrentPos.x > m_DesiredPos.x && m_CurrentPos.y < m_DesiredPos.y)
			{
				m_Jumped = false;
			}
			break;
		case MoveDirections::MoveDownRight:
			break;
		case MoveDirections::MoveUpLeft:
			break;
		case MoveDirections::MoveDownLeft:
			break;
		}
	}
	

}
