#include "LivesComponent.h"
#include "EventQueue.h"
#include "FactionComponent.h"
#include "GameActorComponent.h"
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
		dae::Event deathEvent(dae::Utils::make_sdbm_hash("PlayerDiedEvent"), std::move(payload));

		m_LivesEvent.Notify(GetOwner(), deathEvent);

		if (auto faction = GetOwner()->GetComponent<FactionComponent>())
		{
			if (faction->GetTeam() == Team::Enemy)
			{

				if (auto scoreComp = shooter->GetComponent<ScoreComponent>())
				{
					scoreComp->AddScore(GetOwner()->GetComponent<GameActor>()->GetActorValue());
				}
			}
		}

		auto pl = std::make_unique<ActorDied>(GetOwner());
		dae::Event ActorDiedEvent(dae::Utils::make_sdbm_hash("ActorDied"), std::move(pl));

		if (auto actor = GetOwner()->GetComponent<GameActor>())
		{
			actor->GetEventSubject().Notify(GetOwner(), ActorDiedEvent);
		}
	}
	else
	{
		auto payload = std::make_unique<LivesChangedARGS>(m_Lives);
		dae::Event livesChangedEvent(dae::Utils::make_sdbm_hash("LivesChangedEvent"), std::move(payload));
		m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
	}
}

void LivesComponent::SetHealth(int newLives)
{
	if (m_Lives + newLives > m_MaxLives)
		m_Lives = m_MaxLives;
	else
		m_Lives = newLives;
	auto payload = std::make_unique<LivesChangedARGS>(m_Lives);
	dae::Event livesChangedEvent(dae::Utils::make_sdbm_hash("LivesChangedEvent"), std::move(payload));
	m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
}

void LivesComponent::SetMaxHealth(int newMaxLives)
{
	m_MaxLives = newMaxLives;
}

