#include "Tank_Bullet.h"
#include "GameTime.h"
#include "GameObject.h"
#include "TransformComponent.h"

Tron::TankBullet::TankBullet(dae::GameObject* owner, glm::vec2 velocity) : BaseComponent(owner), m_Velocity(velocity)
{
}
 void Tron::TankBullet::Update()
 {
     auto transform = GetOwner()->GetTransform();
     auto pos = transform->GetLocalPosition();

     pos.x += m_Velocity.x * Time::GetInstance().GetDeltaTime();
     pos.y += m_Velocity.y * Time::GetInstance().GetDeltaTime();

     transform->SetLocalPosition(pos);
 }


