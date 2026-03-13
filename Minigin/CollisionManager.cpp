#include "CollisionManager.h"

#include <memory>

#include "ColliderComponents.h"
#include "EventQueue.h"
#include "Observer.h"

void dae::CollisionManager::AddCollider(ColliderComponent* collider)
{
	if(std::find(m_Colliders.begin(),m_Colliders.end(),collider) == m_Colliders.end())
	{
		m_Colliders.emplace_back(collider);
	}
}

void dae::CollisionManager::RemoveCollider(ColliderComponent* collider)
{
	std::erase(m_Colliders, collider);
}

void dae::CollisionManager::Update()
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            if (m_Colliders[i]->IsOverlapping(m_Colliders[j]))
            {
                EventQueue::GetInstance().AddEvent(
                    std::make_unique<CollisionEvent>(m_Colliders[i], m_Colliders[j])
                );
            }
        }
    }
}