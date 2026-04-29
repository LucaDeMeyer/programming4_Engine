#include "Bullet_CollisionObserver.h"
#include "GameObject.h"
#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "Services.h"
#include "Tank_Bullet.h"
#include "Utils.h"

void Tron::BulletObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID != dae::Utils::make_sdbm_hash("CollisionEvent")) return;

    auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs.get());
    auto* myCollider = GetOwner()->GetComponent<dae::BoxColliderComponent>();

    if (collisionData->Collider1 != myCollider && collisionData->Collider2 != myCollider) return;

    dae::BoxColliderComponent* other = static_cast<dae::BoxColliderComponent*>(
        (collisionData->Collider1 == myCollider) ? collisionData->Collider2 : collisionData->Collider1);

    auto* otherObject = other->GetOwner();
    auto* otherFaction = otherObject->GetComponent<FactionComponent>();

    if (otherFaction && otherFaction->GetTeam() == Team::Wall)
    {
        auto* bulletComp = GetOwner()->GetComponent<TankBullet>();
        if (bulletComp && bulletComp->m_MaxnrBounces > 0)
        {
            bulletComp->Bounce(other->GetWorldBox(), myCollider->GetWorldBox());
          
        }
        else
        {
            auto soundArgs = std::make_unique<dae::SoundARGS>(
                dae::Utils::make_sdbm_hash("Bullet_Explosion"),
                .5f,
                dae::AudioType::FX
            );
            dae::Event audioEvent(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), std::move(soundArgs));
            dae::EventQueue::GetInstance().AddEvent(std::move(audioEvent));
            GetOwner()->MarkForDestruction();
        }
    }

    else if (otherFaction && otherFaction->GetTeam() != GetOwner()->GetComponent<FactionComponent>()->GetTeam())
        GetOwner()->MarkForDestruction();

}
