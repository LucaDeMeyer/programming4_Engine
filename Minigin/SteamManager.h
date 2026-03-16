#ifndef STEAM_MANAGER_H
#define STEAM_MANAGER_H
#include "Singleton.h"

namespace dae
{
	class SteamManager : public Singleton<SteamManager>
	{
    public:
        void Init();
        void Update();
        void Shutdown();

    private:
        friend class Singleton<SteamManager>;
        SteamManager() = default;
        ~SteamManager() = default;

        bool m_bIsInitialized{ false };
    };
}
#endif