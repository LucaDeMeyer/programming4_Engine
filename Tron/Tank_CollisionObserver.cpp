#include "Tank_CollisionObserver.h"
#include "FactionComponent.h"
#include "LivesComponent.h"
#include "Tank_Bullet.h"
#include "GameObject.h"
#include "ColliderComponents.h"

void Tron::TankCollisionObserver::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID == dae::make_sdbm_hash("CollisionEvent"))
    {
        if (auto* collisionData = static_cast<dae::CollisionARGS*>(event.pArgs.get()))
        {
            auto* myCollider = GetOwner()->GetComponent<dae::ColliderComponent>();
            if (!myCollider) return;

            dae::ColliderComponent* otherCollider = nullptr;

            if (collisionData->Collider1 == myCollider) {
                otherCollider = collisionData->Collider2;
            }
            else if (collisionData->Collider2 == myCollider) {
                otherCollider = collisionData->Collider1;
            }

            if (otherCollider)
            {
                dae::GameObject* otherObject = otherCollider->GetOwner();

                if (otherObject->GetComponent<TankBullet>())
                {
                    auto* myFaction = GetOwner()->GetComponent<FactionComponent>();
                    auto* bulletFaction = otherObject->GetComponent<FactionComponent>();

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
                                lives->DoDamage(1);
                            }
                            otherObject->MarkForDestruction(); 
                        }
                    }
                }
            }
        }
    }
}
