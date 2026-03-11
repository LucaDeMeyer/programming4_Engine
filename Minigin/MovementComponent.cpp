#include "MovementComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "GameTime.h"

void dae::MovementComponent::MoveVertical(int direction)
{
    auto pos = GetOwner()->GetTransform()->GetLocalPosition();
    pos.y += direction * m_Speed * Time::GetInstance().GetDeltaTime();
    GetOwner()->GetTransform()->SetLocalPosition(pos);
}

void dae::MovementComponent::MoveHorizontal(int direction)
{
    auto pos = GetOwner()->GetTransform()->GetLocalPosition();
    pos.x += direction * m_Speed * Time::GetInstance().GetDeltaTime();
    GetOwner()->GetTransform()->SetLocalPosition(pos);
}