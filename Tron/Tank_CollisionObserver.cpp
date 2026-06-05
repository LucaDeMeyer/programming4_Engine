#include "Tank_CollisionObserver.h"
#include "Components/AIComponent.h"
#include "Components/FactionComponent.h"
#include "Components/LivesComponent.h"
#include "GameObject.h"
#include "ColliderComponents.h"
#include "LevelManager.h"
#include "Minigin.h"
#include "TransformComponent.h"
#include "TronEvents.h"
#include "Utils.h"
#include "Components/PlayerComponent.h"
#include "Memory/MemoryOverrides.h"
void Tron::TankCollisionObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID != dae::Utils::make_sdbm_hash("CollisionEvent")) return;

    auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs);
    if (!collisionData) return;

    auto* myCollider = GetOwner()->GetComponent<dae::ColliderComponent>();

    if (collisionData->Collider1 != myCollider && collisionData->Collider2 != myCollider)
    {
        return;
    }

    dae::ColliderComponent* otherCollider = (collisionData->Collider1 == myCollider) ? collisionData->Collider2 : collisionData->Collider1;

    if (!otherCollider) return;
    dae::GameObject* otherObject = otherCollider->GetOwner();



	HandleWallCollision(otherObject,myCollider);

    if (otherObject->GetComponent<LivesComponent>())
        HandleTankCollision(otherObject);

    auto* faction = otherObject->GetComponent<FactionComponent>();

    if (faction && faction->GetTeam() == Team::Center && !otherObject->IsMarkedForDestruction())
    {
      
        auto payload = new (dae::Minigin::GetFrameAllocator())Teleport(GetOwner());
        dae::EventQueue::GetInstance().AddEvent(dae::Event(dae::Utils::make_sdbm_hash("Teleport"),payload));
    }
}

void Tron::TankCollisionObserver::HandleWallCollision(dae::GameObject* other, dae::ColliderComponent* triggeredCollider)
{
    auto* otherFaction = other->GetComponent<FactionComponent>();
    if (!otherFaction || otherFaction->GetTeam() != Team::Wall) return;

    auto* myBodyCollider = GetOwner()->GetComponent<dae::ColliderComponent>();
    if (triggeredCollider != myBodyCollider) return;

    if (GetOwner()->GetComponent<AIComponent>()) return;

    auto* transform = GetOwner()->GetTransform();
    transform->SetLocalPosition(transform->GetPreviousPosition());
}

void Tron::TankCollisionObserver::HandleTankCollision(dae::GameObject* other)
{
    auto* myFaction = GetOwner()->GetComponent<FactionComponent>();
    auto* otherFaction = other->GetComponent<FactionComponent>();
    if (!otherFaction || !myFaction) return;

    if (otherFaction->GetTeam() == myFaction->GetTeam() || otherFaction->GetTeam() == Team::Wall) return;

    if (auto* otherAI = other->GetComponent<AIComponent>())
    {
        if (myFaction->GetTeam() == Team::Player1 || myFaction->GetTeam() == Team::Player2)
        {
            if (auto* lives = GetOwner()->GetComponent<LivesComponent>())
            {

                if (auto* playerComp = GetOwner()->GetComponent<PlayerComponent>())
                {
                    if (playerComp->IsInvulnerable()) return;

                    playerComp->TransitionTo(std::make_unique<InvulnerableState>());
                }

                lives->DoDamage(1, other);
            }
        }
    }
}
