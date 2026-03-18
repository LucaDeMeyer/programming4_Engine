#include "LivesComponent.h"

#include <iostream>
#include "EventQueue.h"
#include "FactionComponent.h"
#include "TronEvents.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ScoreComponent.h"

using namespace Tron;
void LivesComponent::DoDamage(int Damage, dae::GameObject* shooter)
{
	m_Lives -= Damage;

	if (m_Lives <= 0)
	{
		auto payload = std::make_unique<PlayerDiedARGS>(1); 
		dae::Event deathEvent(dae::make_sdbm_hash("PlayerDiedEvent"), std::move(payload));

		m_LivesEvent.Notify(GetOwner(), deathEvent);

		if (auto faction = GetOwner()->GetComponent<FactionComponent>())
		{
			if (faction->GetTeam() == Team::Enemy)
			{
				if (auto scoreComp = shooter->GetComponent<ScoreComponent>())
				{
					scoreComp->AddScore(100);
				}
			}
		}

		dae::InputManager::GetInstance().RemoveCommandsForObject(GetOwner()); // this is temp => should both move to somesort of gamemanager that handles this through observer/event Queue
		GetOwner()->MarkForDestruction();
	}
	else
	{
		auto payload = std::make_unique<LivesChangedARGS>(m_Lives);
		dae::Event livesChangedEvent(dae::make_sdbm_hash("LivesChangedEvent"), std::move(payload));
		m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
	}
}

void LivesComponent::SetHealth(int newLives) // not sure if we even need this, havent seen any healing done in gameplay
{
	if (m_Lives + newLives > m_MaxLives)
		m_Lives = m_MaxLives;
	else
		m_Lives = newLives;
	auto payload = std::make_unique<LivesChangedARGS>(m_Lives);
	dae::Event livesChangedEvent(dae::make_sdbm_hash("LivesChangedEvent"), std::move(payload));
	m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
}

void LivesComponent::SetMaxHealth(int newMaxLives)
{
	m_MaxLives = newMaxLives;
}

