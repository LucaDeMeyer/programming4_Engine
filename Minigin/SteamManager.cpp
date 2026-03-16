#include "SteamManager.h"
#include <iostream>
#include <stdexcept>
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

void dae::SteamManager::Init()
{
#if USE_STEAMWORKS
    if (!SteamAPI_Init())
    {
        throw std::runtime_error("Fatal Error - Steam must be running to play this game.");
    }
    std::cout << "Waking up Steam" << std::endl;
    m_bIsInitialized = true;
#endif
}

void dae::SteamManager::Update()
{
#if USE_STEAMWORKS
    if (m_bIsInitialized)
    {
        SteamAPI_RunCallbacks();
    }
#endif
}

void dae::SteamManager::Shutdown()
{
#if USE_STEAMWORKS
    if (m_bIsInitialized)
    {
        SteamAPI_Shutdown();
    }
#endif
}