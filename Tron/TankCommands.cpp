#include "TankCommands.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "LivesComponent.h"
#include "SceneManager.h"
#include "SpriteComponent.h"
#include "TronFactory.h"
using namespace Tron;
void MoveCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj) return;

    auto transform = GetGameObject()->GetTransform();
    auto currentPos = transform->GetLocalPosition();
    float deltaTime = Time::GetInstance().GetDeltaTime();

    currentPos.x += m_Direction.x * deltaTime;
    currentPos.y += m_Direction.y * deltaTime;

    transform->SetLocalPosition(currentPos);

    UpdateSpriteDirection();

}

void MoveCommand::UpdateSpriteDirection()
{
    if (auto sprite = GetGameObject()->GetComponent<dae::SpriteComponent>())
    {
        if (m_Direction.y < 0)      sprite->SetFrame(3); // Up
        else if (m_Direction.x > 0) sprite->SetFrame(2); // Right
        else if (m_Direction.y > 0) sprite->SetFrame(1); // Down
        else if (m_Direction.x < 0) sprite->SetFrame(0); // Left
    }
}


void DamageCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj) return;

    GetGameObject()->GetComponent<LivesComponent>()->DoDamage(m_Damage,obj);
}

void FireCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj) return;

    auto pos = obj->GetTransform()->GetWorldPosition();

    auto factionComp = obj->GetComponent<FactionComponent>();
    Team team = factionComp ? factionComp->GetTeam() : Team::Player1; 

    glm::vec2 velocity{ 0, 0 };
    float bulletSpeed = 300.0f; 

    if (auto sprite = obj->GetComponent<dae::SpriteComponent>())
    {
        int frame = sprite->GetFrame();

        if (frame == 0)      velocity = { -bulletSpeed, 0};
        else if (frame == 1) velocity = { 0, bulletSpeed };
        else if (frame == 2) velocity = { bulletSpeed, 0};
        else if (frame == 3) velocity = { 0, -bulletSpeed };
    }
    auto bullet = GOFactory::CreateBullet(pos, velocity, team,obj);

    dae::SceneManager::GetInstance().GetActiveScene().Add(std::move(bullet));
}