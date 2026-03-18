#ifndef TRON_FACTORY_H
#define TRON_FACTORY_H
#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "SpriteComponent.h"
#include "Tank_Bullet.h"
#include "Tank_CollisionObserver.h"
#include "TextureComponent.h"
#include "glm/vec3.hpp"
#include "TransformComponent.h"
#include "ScoreComponent.h"
namespace Tron
{
    struct PlayerAssembly
    {
        std::unique_ptr<dae::GameObject> Base;
        std::unique_ptr<dae::GameObject> Turret;
    };

    class GOFactory
    {
    public:

        static std::unique_ptr<dae::GameObject> CreateBullet(glm::vec3 startPos, glm::vec2 velocity, Team team, dae::GameObject* shooter)
        {
            startPos.x += 12.0f;
            startPos.y += 12.0f; // offset to have bullet spawn in center, half of tank sprite - half of bullet sprite

            auto bullet = std::make_unique<dae::GameObject>();
            bullet->GetTransform()->SetLocalPosition(startPos);
            if (team == Team::Player1 || team == Team::Player2)
                bullet->AddComponent<dae::TextureComponent>()->SetTexture("Tank_Bullet.png");
            else
                bullet->AddComponent<dae::TextureComponent>()->SetTexture("Bullet_Enemy.png");

            bullet->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, 8, 8 });
            bullet->AddComponent<Tron::TankBullet>(shooter,velocity);
            bullet->AddComponent<Tron::FactionComponent>(team);
            return bullet;
        }

        static PlayerAssembly CreatePlayer(glm::vec3 startPos, const std::string& spriteSheet, Team team)
        {
            auto base = std::make_unique<dae::GameObject>();
            base->GetTransform()->SetLocalPosition(startPos);
            base->AddComponent<dae::SpriteComponent>(spriteSheet, 4, 1);
            base->AddComponent<Tron::LivesComponent>(3);
            base->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0,0,32,32 });
            base->AddComponent<Tron::FactionComponent>(team);
            base->AddComponent<Tron::TankCollisionObserver>();
            base->AddComponent<Tron::ScoreComponent>();

            auto turret = std::make_unique<dae::GameObject>();
            turret->GetTransform()->SetLocalPosition({ -16, -16, 0 });
            turret->AddComponent<dae::SpriteComponent>("Turret_Sheet.png", 10, 4);
            turret->SetParent(base.get(), false);

            return PlayerAssembly{ std::move(base), std::move(turret) };
        }

        static std::unique_ptr<dae::GameObject> CreateEnemy(glm::vec3 startPos)
        {
            auto enemy = std::make_unique<dae::GameObject>();
            enemy->GetTransform()->SetLocalPosition(startPos);
            enemy->AddComponent<dae::SpriteComponent>("BlueTank_SpriteSheet.png", 4, 1);
            enemy->AddComponent<Tron::LivesComponent>(1); 
            enemy->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0,0,32,32 });
            enemy->AddComponent<Tron::FactionComponent>(Team::Enemy);
            enemy->AddComponent<Tron::TankCollisionObserver>();
            return enemy;
        }
    };

}
#endif
