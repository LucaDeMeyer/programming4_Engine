#include "AchievmentManager.h"
#include "GameObject.h"
#include"TronEvents.h"
#include<iostream>

void Tron::AchievementManager::Init()
{

    if (SteamUserStats() == nullptr) return;

    #ifdef _DEBUG
        SteamUserStats()->ResetAllStats(true); // could also add a command to do this during gameplay
        SteamUserStats()->StoreStats();
    #endif
}
void Tron::AchievementManager::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID == dae::make_sdbm_hash("ScoreChangedEvent"))
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
    {
        bool bUnlocked = false;
        if (SteamUserStats()->GetAchievement("ACH_WIN_ONE_GAME", &bUnlocked) && !bUnlocked)
        {
            SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
            SteamUserStats()->StoreStats(); 
        }
    }
}

void Tron::AchievementManager::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
    if (pCallback->m_eResult == k_EResultOK)
    {
        std::cout << "Steam Stats received successfully.\n";
    }
}

void Tron::AchievementManager::AchievementUnlocked(UserAchievementStored_t* pCallback)
{
    std::cout << "Steam confirmed achievement: " << pCallback->m_rgchAchievementName << "\n";
}