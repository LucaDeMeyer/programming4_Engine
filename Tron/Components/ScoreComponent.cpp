#include "ScoreComponent.h"

#include "TronEvents.h"
#include <iostream>

#include "Minigin.h"
#include "Utils.h"
#include "MemoryOverrides.h"
void Tron::ScoreComponent::AddScore(int score)
{
    m_Score += score;
    auto payload = new(dae::Minigin::GetFrameAllocator())ScoreGainedARGS(m_PlayerIndex,m_Score);
    dae::Event scoreEvent(dae::Utils::make_sdbm_hash("ScoreChangedEvent"), std::move(payload));
    m_ScoreEvent.Notify(GetOwner(), scoreEvent);
}
