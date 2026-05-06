#include "States.h"

#include <iostream>

#include "AIComponent.h"
#include "TankCommands.h" 
#include "GameTime.h"

void Tron::PatrolState::OnEnter(AIComponent& ai)
{
    m_MadeDecisionThisTile = false;
}

Tron::EnemyState* Tron::PatrolState::Update(AIComponent& ai)
{
    if (ai.CanSeePlayer())
        return new ChaseState();

    bool atCenter = ai.IsAtTileCenter();
    if (atCenter && !m_MadeDecisionThisTile)
    {
        ai.SnapToGrid();
        if (ai.IsPathBlocked(ai.GetCurrentDirection()))
            ai.ChooseNewDirection();

        m_MadeDecisionThisTile = true;
    }
    else if (!atCenter)
    {
        m_MadeDecisionThisTile = false;
    }

    return nullptr;
}


void Tron::ChaseState::OnEnter(AIComponent& ai)
{
    m_LostSightTimer = 0.f;
    m_MadeDecisionThisTile = false;
    m_LostSightTimer = 0.f;
}

Tron::EnemyState* Tron::ChaseState::Update(AIComponent& ai)
{
    if (!ai.CanSeePlayer())
    {
        m_LostSightTimer += Time::GetInstance().GetDeltaTime();
        if (m_LostSightTimer >= k_LostSightTimeout)
            return new PatrolState();
    }
    else
    {
        m_LostSightTimer = 0.f;
    }

    bool atCenter = ai.IsAtTileCenter();

    if (atCenter && !m_MadeDecisionThisTile)
    {
        ai.SnapToGrid();
        ai.ChasePlayer();
        m_MadeDecisionThisTile = true;
    }
    else if (!atCenter)
    {
        m_MadeDecisionThisTile = false;
    }

    return nullptr;
}