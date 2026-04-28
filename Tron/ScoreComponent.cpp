#include "ScoreComponent.h"

#include "TronEvents.h"
#include <iostream>

#include "Utils.h"

void Tron::ScoreComponent::AddScore(int score)
{
    m_Score += score;
    auto payload = std::make_unique<ScoreGainedARGS>(m_Score); 
    dae::Event scoreEvent(dae::Utils::make_sdbm_hash("ScoreChangedEvent"), std::move(payload));
    m_ScoreEvent.Notify(GetOwner(), scoreEvent);
}
