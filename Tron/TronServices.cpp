#include "TronServices.h"

#include <iostream>

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif
void Tron::SteamAchievementService::UnlockAchievement(const char* id)
{
#if USE_STEAMWORKS
    bool bUnlocked = false;
    if (SteamUserStats()->GetAchievement(id, &bUnlocked) && !bUnlocked)
    {
        SteamUserStats()->SetAchievement(id);
        SteamUserStats()->StoreStats();
    }
#endif
}

void Tron::SteamPlatformService::Init()
{
#if USE_STEAMWORKS
    if (!SteamAPI_Init())
    {
        std::cerr << "[Steam] Fatal Error: SteamAPI_Init() failed. Make sure Steam is running and steam_appid.txt is present!\n";
    }
    else
    {
        std::cout << "[Steam] Initialized successfully.\n";
    }
#endif
}

void Tron::SteamPlatformService::Update()
{
#if USE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif
}

void Tron::SteamPlatformService::Shutdown()
{
#if USE_STEAMWORKS
    SteamAPI_Shutdown();
    std::cout << "[Steam] Shutdown.\n";
#endif
}