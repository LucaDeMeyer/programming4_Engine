#ifndef TRON_SERVICES_H
#define TRON_SERVICES_H
#include "Managers/BulletManager.h"
#include "Managers/LevelManager.h"
#include "Managers/ParticleManager.h"
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

    class TronGameLoopService final : public dae::IGameService
    {
    public:
        void Update() override
        {
            Tron::LevelManager::GetInstance().Update();
            Tron::BulletManager::GetInstance().Update();
            Tron::ParticleManager::GetInstance().Update();
        }

        void Render() const override
        {
            Tron::BulletManager::GetInstance().Render();
            Tron::ParticleManager::GetInstance().Render();
        }
    };
}
#endif