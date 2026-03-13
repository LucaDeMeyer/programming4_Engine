#include "LivesComponent.h"

#include <windows.h>

#include "TronEvents.h"
#include "GameObject.h"


void LivesComponent::DoDamage(int Damage)
{
	if (m_Lives - Damage <= 0)
	{
		m_LivesEvent.Notify(GetOwner(), PlayerDied{ 1 });
	}
	else
	{
		m_Lives -= Damage;
		m_LivesEvent.Notify(GetOwner(), LivesChanged{m_Lives});

	}
}

void LivesComponent::SetHealth(int newLives)
{
	if (m_Lives + newLives > m_MaxLives)
		m_Lives = m_MaxLives;
	else
		m_Lives = newLives;

	m_LivesEvent.Notify(GetOwner(), LivesChanged(m_Lives));
}

void LivesComponent::SetMaxHealth(int newMaxLives)
{
	m_MaxLives = newMaxLives;
}

