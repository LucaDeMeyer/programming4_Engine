#include "LivesComponent.h"

#include <iostream>
#include <windows.h>

#include "EventQueue.h"
#include "FactionComponent.h"
#include "TronEvents.h"
#include "GameObject.h"
using namespace Tron;
void LivesComponent::DoDamage(int Damage)
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
				auto scorePayload = std::make_unique<ScoreGainedARGS>(100); 
				dae::Event scoreEvent(dae::make_sdbm_hash("ScoreGainedEvent"), std::move(scorePayload));
				dae::EventQueue::GetInstance().AddEvent(std::move(scoreEvent));
			}
		}
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

