#include "TankCommands.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "LivesComponent.h"

void MoveCommand::Execute()
{

    auto transform = GetGameObject()->GetTransform();
    auto currentPos = transform->GetLocalPosition();
    float deltaTime = Time::GetInstance().GetDeltaTime();

    currentPos.x += m_Direction.x * deltaTime;
    currentPos.y += m_Direction.y * deltaTime;

    transform->SetLocalPosition(currentPos);

}

void DamageCommand::Execute()
{
    GetGameObject()->GetComponent<LivesComponent>()->DoDamage(m_Damage);
}

