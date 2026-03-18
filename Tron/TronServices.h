#ifndef TRON_SERVICES_H
#define TRON_SERVICES_H
#include "Services.h"

namespace Tron
{
    class SteamAchievementService final : public dae::IAchievementService {
    public:
        void UnlockAchievement(const char* id) override;
    };

    class SteamPlatformService final : public dae::IPlatformService {
    public:
        void Init() override;
        void Update() override;
        void Shutdown() override;
    };
}
#endif