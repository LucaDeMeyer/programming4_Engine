#include "Bullet_CollisionObserver.h"
#include "GameObject.h"
#include "ColliderComponents.h"
#include "Tank_Bullet.h"

void Tron::BulletObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
	if (event.ID != dae::make_sdbm_hash("CollisionEvent")) return;

    auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs.get());
    if (!collisionData) return;

    auto* myCollider = GetOwner()->GetComponent<dae::ColliderComponent>();
    dae::ColliderComponent* otherCollider = (collisionData->Collider1 == myCollider) ? collisionData->Collider2 : collisionData->Collider1;

    if (!otherCollider) return;
    dae::GameObject* otherObject = otherCollider->GetOwner();

    auto* bulletComp = GetOwner()->GetComponent<TankBullet>();
    if (bulletComp && otherObject == bulletComp->GetShooter()) return;

    GetOwner()->MarkForDestruction();
}
