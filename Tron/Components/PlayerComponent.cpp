#include "PlayerComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "../States.h"
#include "Utils.h"

Tron::PlayerComponent::PlayerComponent(dae::GameObject* owner,int playerIdx)
    : BaseComponent(owner), m_CurrentState(std::make_unique<NormalPlayerState>()),m_PlayerIndex(playerIdx)
{
    m_SpawnLocation = owner->GetTransform()->GetLocalPosition();
}

void Tron::PlayerComponent::Update()
{
    if (!m_CurrentState) return;

    auto nextState = m_CurrentState->Update(*this);
    if (nextState)
    {
        TransitionTo(std::move(nextState));
    }
}

void Tron::PlayerComponent::TransitionTo(std::unique_ptr<PlayerState> newState)
{
    if (m_CurrentState) m_CurrentState->OnExit(*this);
    m_CurrentState = std::move(newState);
    m_CurrentState->OnEnter(*this);
}

void Tron::PlayerComponent::RespawnAtStart()
{
    GetOwner()->GetTransform()->SetLocalPosition(m_SpawnLocation);
}

void Tron::PlayerComponent::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (obj != GetOwner()) return;

    if (event.ID == dae::Utils::make_sdbm_hash("LivesChangedEvent"))
    {
        TransitionTo(std::make_unique<InvulnerableState>());
    }
}
