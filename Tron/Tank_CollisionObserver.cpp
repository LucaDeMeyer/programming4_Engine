#include "Tank_CollisionObserver.h"

#include <iostream>

#include "FactionComponent.h"
#include "LivesComponent.h"
#include "Tank_Bullet.h"
#include "GameObject.h"
#include "ColliderComponents.h"
#include "LevelManager.h"
#include "TransformComponent.h"
#include "TronEvents.h"

void Tron::TankCollisionObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID != dae::make_sdbm_hash("CollisionEvent")) return;

    auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs.get());
    if (!collisionData) return;

    auto* myCollider = GetOwner()->GetComponent<dae::ColliderComponent>();

    if (collisionData->Collider1 != myCollider && collisionData->Collider2 != myCollider)
    {
        return;
    }

    dae::ColliderComponent* otherCollider = (collisionData->Collider1 == myCollider) ? collisionData->Collider2 : collisionData->Collider1;

    if (!otherCollider) return;
    dae::GameObject* otherObject = otherCollider->GetOwner();

    if (otherObject->GetComponent<TankBullet>())
		HandleBulletCollisions(otherObject);


	HandleWallCollision(otherObject,myCollider);

    auto* faction = otherObject->GetComponent<FactionComponent>();

    if (faction && faction->GetTeam() == Team::Center && !otherObject->IsMarkedForDestruction())
    {
      
        auto payload = std::make_unique<::Teleport>(GetOwner());

        dae::EventQueue::GetInstance().AddEvent(dae::Event(
            dae::make_sdbm_hash("Teleport"),
            std::move(payload)
        ));
    }
}

void Tron::TankCollisionObserver::HandleBulletCollisions(dae::GameObject* other)
{
   auto* myFaction = GetOwner()->GetComponent<FactionComponent>();
   auto* bulletFaction = other->GetComponent<FactionComponent>();
   if (myFaction && bulletFaction)
   {
       Team me = myFaction->GetTeam();
       Team bulletTeam = bulletFaction->GetTeam();

       bool shouldTakeDamage = false;
       if (bulletTeam == Team::Enemy)
       {
           shouldTakeDamage = true;
       }
       else if (bulletTeam != me)
       {
           shouldTakeDamage = true;
       }

       if (shouldTakeDamage)
       {
           if (auto* lives = GetOwner()->GetComponent<LivesComponent>()) {

               dae::GameObject* shooter = nullptr;
               if (auto* bulletComp = other->GetComponent<TankBullet>())
               {
                   shooter = bulletComp->GetShooter();
               }
               lives->DoDamage(1, shooter);
           }
       }
   }
}

void Tron::TankCollisionObserver::HandleWallCollision(dae::GameObject* other, dae::ColliderComponent* triggeredCollider)
{
    auto* otherFaction = other->GetComponent<FactionComponent>();
    if (!otherFaction || otherFaction->GetTeam() != Team::Wall) return;

    auto* myBodyCollider = GetOwner()->GetComponent<dae::ColliderComponent>();
    if (triggeredCollider != myBodyCollider) return;

    auto* transform = GetOwner()->GetTransform();
    transform->SetLocalPosition(transform->GetPreviousPosition());
}

