#include "LivesComponent.h"

#include <iostream>
#include <windows.h>

#include "TronEvents.h"
#include "GameObject.h"
void LivesComponent::DoDamage(int Damage)
{
	if (m_Lives - Damage == 0)
	{
		auto payload = std::make_unique<PlayerDiedARGS>(1); 
		dae::Event deathEvent(dae::make_sdbm_hash("PlayerDiedEvent"), std::move(payload));

		m_LivesEvent.Notify(GetOwner(), deathEvent);
	}
	else
	{
		auto payload = std::make_unique<LivesChangedARGS>(m_Lives);
		dae::Event livesChangedEvent(dae::make_sdbm_hash("LivesChangedEvent"), std::move(payload));

		m_Lives -= Damage;
		m_LivesEvent.Notify(GetOwner(), livesChangedEvent);

	
		std::cout << m_Lives << std::endl;
	}
}

void LivesComponent::SetHealth(int newLives)
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

