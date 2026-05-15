#include "Bullet_CollisionObserver.h"

#include <iostream>
#include <ostream>

#include "GameObject.h"
#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "Services.h"
#include "Tank_Bullet.h"
#include "Utils.h"

void Tron::BulletObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID != dae::Utils::make_sdbm_hash("CollisionEvent")) return;


    auto* bullet = GetOwner();
    if (!bullet) return;

    auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs.get());
    auto* myCollider = bullet->GetComponent<dae::BoxColliderComponent>();
 
    dae::BoxColliderComponent* otherCollider = nullptr;
    if (collisionData->Collider1 == myCollider)
        otherCollider = static_cast<dae::BoxColliderComponent*>(collisionData->Collider2);
    else if (collisionData->Collider2 == myCollider)
        otherCollider = static_cast<dae::BoxColliderComponent*>(collisionData->Collider1);

    if (!otherCollider) return;

    auto* otherObject = otherCollider->GetOwner();
    auto* bulletFaction = bullet->GetComponent<FactionComponent>();
    auto* otherFaction = otherObject->GetComponent<FactionComponent>();

    if (!bulletFaction) return;

    Team myTeam = bulletFaction->GetTeam();

    if (otherFaction && otherFaction->GetTeam() == Team::Wall)
    {
        auto* bulletComp = bullet->GetComponent<TankBullet>();
        if (bulletComp && bulletComp->m_MaxnrBounces > 0)
        {
            bulletComp->Bounce(otherCollider->GetWorldBox(), myCollider->GetWorldBox());
            return; 
        }

        DestroyBullet();
        return;
    }

    if (otherFaction)
    {
        Team theirTeam = otherFaction->GetTeam();

        if (theirTeam == myTeam)
        {
            return;
        }


        DestroyBullet();
    }
}


void Tron::BulletObserver::DestroyBullet()
{
    auto soundArgs = std::make_unique<dae::SoundARGS>(
        dae::Utils::make_sdbm_hash("Bullet_Explosion"), .5f, dae::AudioType::FX
    );
    dae::EventQueue::GetInstance().AddEvent(dae::Event(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), std::move(soundArgs)));

    GetOwner()->MarkForDestruction();
}