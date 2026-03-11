#include "ActorCommands.h"

#include "MovementComponent.h"

void dae::MoveVertical::Execute()
{
    GameObject* obj = GetGameObject();

    if (auto moveComp = obj->GetComponent<MovementComponent>())
    {
        moveComp->MoveVertical(m_Direction);
    }
}

void dae::MoveHorizontal::Execute()
{
	GameObject* obj = GetGameObject();

    if (auto moveComp = obj->GetComponent<MovementComponent>())
    {
        moveComp->MoveHorizontal(m_Direction);
    }
}

