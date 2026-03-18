#include "ColliderComponents.h"
#include "GameObject.h"
#include "TransformComponent.h"
glm::vec4  dae::BoxColliderComponent::GetWorldBox() const
{
    // 1. Get the object's current position in the world
    auto transform = GetOwner()->GetTransform();
    auto pos = transform->GetWorldPosition();

    // 2. Add the local offset (x, y) to the world position. 
    // z is width, w is height.
    return glm::vec4{
        pos.x + m_LocalBox.x,
        pos.y + m_LocalBox.y,
        m_LocalBox.z,
        m_LocalBox.w
    };
}

bool dae::BoxColliderComponent::IsOverlapping(ColliderComponent* other) const
{
    if (const auto* otherBox = dynamic_cast<const BoxColliderComponent*>(other))
    {
        glm::vec4 box1 = GetWorldBox();
        glm::vec4 box2 = otherBox->GetWorldBox();

        return (box1.x < box2.x + box2.z &&
            box1.x + box1.z > box2.x &&
            box1.y < box2.y + box2.w &&
            box1.y + box1.w > box2.y);
    }


    return false; // not handleing circular collisions yet
}