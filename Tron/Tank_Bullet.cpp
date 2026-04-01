#include "Tank_Bullet.h"
#include "GameTime.h"
#include "GameObject.h"
#include "TransformComponent.h"

Tron::TankBullet::TankBullet(dae::GameObject* owner, dae::GameObject* shooter,glm::vec2 velocity) : BaseComponent(owner), m_Shooter(shooter), m_Velocity(velocity)
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

 void Tron::TankBullet::Bounce(const glm::vec4& wallBox, const glm::vec4& bulletBox)
 {
     if (m_MaxnrBounces > 0)
     {

         float overlapX = std::min(bulletBox.x + bulletBox.z, wallBox.x + wallBox.z) - std::max(bulletBox.x, wallBox.x);
         float overlapY = std::min(bulletBox.y + bulletBox.w, wallBox.y + wallBox.w) - std::max(bulletBox.y, wallBox.y);

         if (overlapX < overlapY)
         {
             m_Velocity.x *= -1;
         }
         else
         {
             m_Velocity.y *= -1;
         }
        m_MaxnrBounces -= 1;
     }
 }

