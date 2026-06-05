#include "TankCommands.h"
#include "BulletManager.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "InputManager.h"
#include "LivesComponent.h"
#include "SpriteComponent.h"
#include "TronFactory.h"
#include "EventQueue.h"
#include "Minigin.h"
#include "MemoryOverrides.h"
#include "LevelManager.h"
using namespace Tron;
void MoveCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj || obj->IsMarkedForDestruction()) return;

    auto transform = obj->GetTransform();
    auto currentPos = transform->GetLocalPosition();
    transform->SetPreviousPosition(currentPos);

    float deltaTime = Time::GetInstance().GetDeltaTime();

    currentPos.x += m_Direction.x *(deltaTime );
    currentPos.y += m_Direction.y * (deltaTime);
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

    Tron::BulletManager::GetInstance().FireBullet(pos, velocity, team, obj);
    auto soundArgs = new(dae::Minigin::GetFrameAllocator())dae::SoundARGS(dae::Utils::make_sdbm_hash("tank_fire"),
        .5f, dae::AudioType::FX);
    dae::Event audioEvent(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), soundArgs);
    dae::EventQueue::GetInstance().AddEvent(std::move(audioEvent));
}

void PlayerFireCommand::Execute()
{
    auto base = GetGameObject();
    if (!base || !m_Turret) return;

    auto center = base->GetTransform()->GetWorldPosition();
   
    auto factionComp = base->GetComponent<FactionComponent>();
    Team team = factionComp ? factionComp->GetTeam() : Team::Player1;

    float angleDegrees = 0.0f;
    if (auto sprite = m_Turret->GetComponent<dae::SpriteComponent>())
    {
        int frame = sprite->GetFrame();

        if (frame >= 0 && frame <= 9)        angleDegrees = -(frame * 9.0f);
        else if (frame >= 10 && frame <= 19) angleDegrees = -90.0f - ((frame - 10) * 9.0f);
        else if (frame >= 20 && frame <= 29) angleDegrees = 90.0f + ((frame - 20) * 9.0f);
        else if (frame >= 30 && frame <= 39) angleDegrees = (frame - 30) * 9.0f;

        if (frame == 9) angleDegrees = -90.0f;
        if (frame == 19) angleDegrees = -180.0f;
        if (frame == 29) angleDegrees = 180.0f;
        if (frame == 39) angleDegrees = 90.0f;
    }

    float angleRadians = angleDegrees * (3.1415926535f / 180.0f);

    float bulletSpeed = 300.0f;
    glm::vec2 velocity{
        std::cos(angleRadians) * bulletSpeed,
        std::sin(angleRadians) * bulletSpeed
    };
    float barrelLength = 24.0f;
    glm::vec3 spawnPos{
         center.x + (std::cos(angleRadians) * barrelLength) - 4.0f,
        center.y + (std::sin(angleRadians) * barrelLength) - 4.0f,
        center.z
    };

    Tron::BulletManager::GetInstance().FireBullet(spawnPos, velocity, team, base);
    auto soundArgs = new(dae::Minigin::GetFrameAllocator())dae::SoundARGS(dae::Utils::make_sdbm_hash("tank_fire"),
        .5f,dae::AudioType::FX);
    dae::Event audioEvent(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), soundArgs);
    dae::EventQueue::GetInstance().AddEvent(std::move(audioEvent));
}

void Tron::PlayerMoveCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj || obj->IsMarkedForDestruction()) return;

    auto transform = obj->GetTransform();
    auto currentPos = transform->GetLocalPosition();
    transform->SetPreviousPosition(currentPos);

    float deltaTime = Time::GetInstance().GetDeltaTime();

    float tileSize = 32.0f;
    auto& levelManager = Tron::LevelManager::GetInstance();
    float offsetX = levelManager.GetOffsetX();
    float offsetY = levelManager.GetOffsetY();

    float snapThreshold = 10.0f; 

    if (std::abs(m_Direction.x) > 0)
    {
        float exactY = std::round((currentPos.y - offsetY) / tileSize) * tileSize + offsetY;

        if (std::abs(currentPos.y - exactY) < snapThreshold)
        {
            currentPos.y = exactY;
        }
    }
   
    else if (std::abs(m_Direction.y) > 0)
    {
        float exactX = std::round((currentPos.x - offsetX) / tileSize) * tileSize + offsetX;

        if (std::abs(currentPos.x - exactX) < snapThreshold)
        {
            currentPos.x = exactX;
        }
    }

    currentPos.x += m_Direction.x * deltaTime;
    currentPos.y += m_Direction.y * deltaTime;

    transform->SetLocalPosition(currentPos);
    UpdateSpriteDirection();
}

void Tron::PlayerMoveCommand::UpdateSpriteDirection()
{
    if (auto sprite = GetGameObject()->GetComponent<dae::SpriteComponent>())
    {
        if (m_Direction.y < 0)      sprite->SetFrame(3); // Up
        else if (m_Direction.x > 0) sprite->SetFrame(2); // Right
        else if (m_Direction.y > 0) sprite->SetFrame(1); // Down
        else if (m_Direction.x < 0) sprite->SetFrame(0); // Left
    }
}

void AimCommand::Execute()
{
    auto obj = GetGameObject();
    if (!obj || obj->IsMarkedForDestruction()) return;

    float targetAngleRadians = 0.0f;
    bool hasAimInput = false;

    if (m_ControllerIndex >= 0)
    {
        glm::vec2 rightStick = dae::InputManager::GetInstance().GetRightThumbstick(m_ControllerIndex);

        if (std::abs(rightStick.x) > 0.1f || std::abs(rightStick.y) > 0.1f)
        {
            targetAngleRadians = std::atan2(rightStick.y, rightStick.x);
            hasAimInput = true;
        }
    }
    else
    {
        glm::vec2 mousePos = dae::InputManager::GetInstance().GetMousePosition();
        glm::vec3 turretWorldPos = obj->GetTransform()->GetWorldPosition();

        float deltaX = mousePos.x - (turretWorldPos.x + 32.0f);
        float deltaY = mousePos.y - (turretWorldPos.y + 32.0f);

        targetAngleRadians = std::atan2(deltaY, deltaX);
        hasAimInput = true;
    }

    if (hasAimInput)
    {
        float angleDegrees = targetAngleRadians * (180.0f / 3.1415926535f);
        int targetFrame = 0;

        if (angleDegrees >= 0.0f && angleDegrees <= 90.0f)
        {
            targetFrame = 30 + static_cast<int>(std::round(angleDegrees / 9.0f));
            if (targetFrame > 39) targetFrame = 39; 
        }
        else if (angleDegrees > 90.0f && angleDegrees <= 180.0f)
        {
            targetFrame = 20 + static_cast<int>(std::round((angleDegrees - 90.0f) / 9.0f));
            if (targetFrame > 29) targetFrame = 29;
        }
        else if (angleDegrees < 0.0f && angleDegrees >= -90.0f)
        {
            targetFrame = 0 + static_cast<int>(std::round(std::abs(angleDegrees) / 9.0f));
            if (targetFrame > 9) targetFrame = 9;
        }
        else if (angleDegrees < -90.0f && angleDegrees >= -180.0f)
        {
            targetFrame = 10 + static_cast<int>(std::round((std::abs(angleDegrees) - 90.0f) / 9.0f));
            if (targetFrame > 19) targetFrame = 19;
        }

        if (auto sprite = obj->GetComponent<dae::SpriteComponent>())
        {
            sprite->SetFrame(targetFrame);
        }
    }
}

void PlayerAimCommand::Execute()
{
    if (!m_Turret || m_Turret->IsMarkedForDestruction()) return;

    if (m_Direction.x == 0 && m_Direction.y == 0) return;

    float targetAngleRadians = std::atan2(m_Direction.y, m_Direction.x);
    float angleDegrees = targetAngleRadians * (180.0f / 3.1415926535f);

    int targetFrame = 0;

    if (angleDegrees >= 0.0f && angleDegrees <= 90.0f) {
        targetFrame = 30 + static_cast<int>(std::round(angleDegrees / 9.0f));
        if (targetFrame > 39) targetFrame = 39;
    }
    else if (angleDegrees > 90.0f && angleDegrees <= 180.0f) {
        targetFrame = 20 + static_cast<int>(std::round((angleDegrees - 90.0f) / 9.0f));
        if (targetFrame > 29) targetFrame = 29;
    }
    else if (angleDegrees < 0.0f && angleDegrees >= -90.0f) {
        targetFrame = 0 + static_cast<int>(std::round(std::abs(angleDegrees) / 9.0f));
        if (targetFrame > 9) targetFrame = 9;
    }
    else if (angleDegrees < -90.0f && angleDegrees >= -180.0f) {
        targetFrame = 10 + static_cast<int>(std::round((std::abs(angleDegrees) - 90.0f) / 9.0f));
        if (targetFrame > 19) targetFrame = 19;
    }

    if (auto sprite = m_Turret->GetComponent<dae::SpriteComponent>()) {
        sprite->SetFrame(targetFrame);
    }
}
