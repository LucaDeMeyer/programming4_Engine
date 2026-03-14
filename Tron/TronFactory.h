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
        static std::unique_ptr<dae::GameObject> CreateBullet(glm::vec3 startPos, glm::vec2 velocity, Team team)
        {
            auto bullet = std::make_unique<dae::GameObject>();
            bullet->GetTransform()->SetLocalPosition(startPos);
            bullet->AddComponent<dae::TextureComponent>()->SetTexture("Tank_Bullet.png");
            bullet->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, 8, 8 });
            bullet->AddComponent<Tron::TankBullet>(velocity);
            bullet->AddComponent<Tron::FactionComponent>(team);
            return bullet;
        }

        static PlayerAssembly CreatePlayer(glm::vec3 startPos)
        {
            auto base = std::make_unique<dae::GameObject>();
            base->GetTransform()->SetLocalPosition(startPos);
            base->AddComponent<dae::SpriteComponent>("RedTank_SpriteSheet.png", 4, 1);
            base->AddComponent<Tron::LivesComponent>(3);
            base->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0,0,32,32 });
            base->AddComponent<Tron::FactionComponent>(Team::Player1);
            base->AddComponent<Tron::TankCollisionObserver>();

            auto turret = std::make_unique<dae::GameObject>();
            turret->GetTransform()->SetLocalPosition({ 8, 8, 0 });
            turret->AddComponent<dae::SpriteComponent>("Turret_SpriteSheet.png", 10, 4);
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
