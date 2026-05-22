#include "ExplosionComponent.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "SpriteComponent.h"
#include "TronEvents.h"
#include "Utils.h"

Tron::ExplosionComponent::ExplosionComponent(dae::GameObject* owner) : BaseComponent(owner), Observer() 
{
	m_Sprite = std::make_unique<dae::SpriteComponent>(owner,"Explosion.png",4,1,4);
	m_Sprite->SetVisible(false);
}

void Tron::ExplosionComponent::OnNotify(dae::GameObject* obj, const dae::Event& event)
{

	if (event.ID == dae::Utils::make_sdbm_hash("LivesChangedEvent"))
	{
		auto* data = static_cast<LivesChangedARGS*>(event.pArgs.get());
		if (data->currentLives > 0)
		{
			m_Sprite->SetVisible(true);
			m_Sprite->SetFrame(0);
		}
	}

	if (event.ID == dae::Utils::make_sdbm_hash("ActorDied"))
	{
		GetOwner()->SetParent(nullptr, true);

		m_Sprite->SetVisible(true);
		m_Sprite->SetFrame(0);
		m_Sprite->Play(.3f, false);
		m_WasDestruction = true;
	}
}

void Tron::ExplosionComponent::Render() const
{
	if (m_Sprite->IsVisible())
		m_Sprite->Render();
}

void Tron::ExplosionComponent::Update()
{
	if (m_Sprite->IsVisible())
	{
		m_Sprite->Update();
		if (!m_Sprite->IsPlaying()&&m_WasDestruction)
		{
			GetOwner()->MarkForDestruction();
		}

	}
}
