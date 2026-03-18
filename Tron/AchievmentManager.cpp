#include "AchievmentManager.h"
#include "GameObject.h"
#include"TronEvents.h"

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include<iostream>

class Tron::AchievementManager::AchievementManagerImpl
{
public:
#if USE_STEAMWORKS
    STEAM_CALLBACK(AchievementManagerImpl, OnUserStatsReceived, UserStatsReceived_t);
    STEAM_CALLBACK(AchievementManagerImpl, OnAchievementUnlocked, UserAchievementStored_t);

    void UnlockAchievement(const char* id)
    {
        bool bUnlocked = false;
        if (SteamUserStats()->GetAchievement(id, &bUnlocked) && !bUnlocked)
        {
            SteamUserStats()->SetAchievement(id);
            SteamUserStats()->StoreStats();
        }
    }
#else
    void UnlockAchievement(const char*) {} 
#endif
};

#if USE_STEAMWORKS
void Tron::AchievementManager::AchievementManagerImpl::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
    if (pCallback->m_eResult == k_EResultOK)
        std::cout << "Steam stats received.\n";
}

void Tron::AchievementManager::AchievementManagerImpl::OnAchievementUnlocked(UserAchievementStored_t* pCallback)
{
    std::cout << "Achievement unlocked: " << pCallback->m_rgchAchievementName << "\n";
}
#endif

Tron::AchievementManager::AchievementManager()
    : m_Pimpl(std::make_unique<AchievementManagerImpl>())
{
}

Tron::AchievementManager::~AchievementManager() = default;

void Tron::AchievementManager::Init()
{
#if USE_STEAMWORKS
    if (SteamUserStats())
    {
#ifdef _DEBUG
        SteamUserStats()->ResetAllStats(true);
        SteamUserStats()->StoreStats();
#endif
    }
#endif
}

void Tron::AchievementManager::OnNotify(dae::GameObject*, const dae::Event& event)
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
        m_Pimpl->UnlockAchievement("ACH_WIN_ONE_GAME");
}