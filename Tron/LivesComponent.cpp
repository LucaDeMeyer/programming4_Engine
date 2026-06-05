#include "LivesComponent.h"
#include "EventQueue.h"
#include "FactionComponent.h"
#include "GameActorComponent.h"
#include "TronEvents.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ScoreComponent.h"
#include "Services.h"
#include "Memory/MemoryOverrides.h"
using namespace Tron;
void LivesComponent::DoDamage(int Damage, dae::GameObject* shooter)
{
	m_Lives -= Damage;

	if (m_Lives <= 0)
	{
		auto payload = new (dae::Minigin::GetFrameAllocator())PlayerDiedARGS(1); 
		dae::Event deathEvent(dae::Utils::make_sdbm_hash("PlayerDiedEvent"),payload);

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

		auto pl = new (dae::Minigin::GetFrameAllocator())ActorDied(GetOwner());
		dae::Event ActorDiedEvent(dae::Utils::make_sdbm_hash("ActorDied"), pl);

		m_LivesEvent.Notify(GetOwner(), ActorDiedEvent);

		auto soundArgs = new (dae::Minigin::GetFrameAllocator())dae::SoundARGS(
			dae::Utils::make_sdbm_hash("Tank_Explosion"),
			.5f,
			dae::AudioType::FX
		);
		dae::Event audioEvent(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), soundArgs);
		dae::EventQueue::GetInstance().AddEvent(std::move(audioEvent));

		if (auto actor = GetOwner()->GetComponent<GameActor>())
		{
			actor->GetEventSubject().Notify(GetOwner(), ActorDiedEvent);
		}
	}
	else
	{
		auto payload = new (dae::Minigin::GetFrameAllocator())LivesChangedARGS(m_Lives,playerID);
		dae::Event livesChangedEvent(dae::Utils::make_sdbm_hash("LivesChangedEvent"), payload);
		m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
	}
}

void LivesComponent::SetHealth(int newLives)
{
	if (newLives > m_MaxLives)
		m_Lives = m_MaxLives;
	else
		m_Lives = newLives;
	auto payload = new (dae::Minigin::GetFrameAllocator())LivesChangedARGS(m_Lives, playerID);
	dae::Event livesChangedEvent(dae::Utils::make_sdbm_hash("LivesChangedEvent"), payload);
	m_LivesEvent.Notify(GetOwner(), livesChangedEvent);
}

void LivesComponent::SetMaxHealth(int newMaxLives)
{
	m_MaxLives = newMaxLives;
}

