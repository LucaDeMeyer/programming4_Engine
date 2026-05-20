#include "States.h"

#include <iostream>

#include "AIComponent.h"
#include "TankCommands.h" 
#include "GameTime.h"
std::unique_ptr<Tron::EnemyState> Tron::PatrolState::Update(AIComponent& ai)
{
   
    if (ai.CanSeePlayer())
    {
        
        return ai.GetAggroState();
    }

    ai.HandlePatrol();

    return nullptr;
}
void Tron::ChaseState::OnEnter(AIComponent& ai)
{
    m_LostSightTimer = 0.f;
}

std::unique_ptr<Tron::EnemyState> Tron::ChaseState::Update(AIComponent& ai)
{
    if (!ai.CanSeePlayer())
    {
        m_LostSightTimer += Time::GetInstance().GetDeltaTime();
        if (m_LostSightTimer >= k_LostSightTimeout)
        {
            return std::make_unique<PatrolState>(); 
        }
    }
    else
    {
        m_LostSightTimer = 0.f;
    }

  
    ai.HandleChase();

    return nullptr;
}
std::unique_ptr<Tron::EnemyState> Tron::AttackState::Update(AIComponent& ai)
{
    if (!ai.CanSeePlayer())
    {
        return std::make_unique<PatrolState>();
    }

    ai.HandlePatrol(); 
    ai.HandleAttack();

    return nullptr;
}