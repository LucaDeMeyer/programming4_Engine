
#include "AchievmentManager.h"
#include "GameObject.h"
#include "TronEvents.h"
#include "ServiceLocator.h" 
#include "Utils.h"

void Tron::AchievementManager::OnNotify(dae::GameObject*, const dae::Event& event)
{
    if (event.ID == dae::Utils::make_sdbm_hash("ScoreChangedEvent"))
    {
        if (auto* payload = static_cast<ScoreGainedARGS*>(event.pArgs.get()))
        {
            m_CurrentScore = payload->points;
            CheckUnlockCondition();
        }
    }
}

void Tron::AchievementManager::CheckUnlockCondition()
{
    if (m_CurrentScore >= WINNER_THRESHOLD)
        dae::ServiceLocator::GetAchievements().UnlockAchievement("ACH_WIN_ONE_GAME");
}
