#include "CollisionManager.h"

#include <iostream>
#include <memory>

#include "ColliderComponents.h"
#include "EventQueue.h"
#include "Observer.h"
#include "Event.h"
void dae::CollisionManager::AddCollider(ColliderComponent* collider)
{
	if(std::find(m_Colliders.begin(),m_Colliders.end(),collider) == m_Colliders.end())
	{
		m_Colliders.emplace_back(collider);
        std::cout << "colliders added: " << m_Colliders.size() << '\n';
	}
}

void dae::CollisionManager::RemoveCollider(ColliderComponent* collider)
{
    m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), collider),m_Colliders.end());
    std::cout << "colliders Left: " << m_Colliders.size() << '\n';
}

void dae::CollisionManager::Update()
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        for (size_t j = i + 1; j < m_Colliders.size(); ++j)
        {
            if (m_Colliders[i]->IsOverlapping(m_Colliders[j]))
            {
                auto payload = std::make_unique<dae::CollisionARGS>(m_Colliders[i], m_Colliders[j]);
                dae::Event hitEvent(make_sdbm_hash("CollisionEvent"), std::move(payload));
               EventQueue::GetInstance().AddEvent(std::move(hitEvent));
            }
        }
    }
}