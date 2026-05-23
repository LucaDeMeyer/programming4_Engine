#ifndef TRON_SERVICES_H
#define TRON_SERVICES_H
#include "BulletManager.h"
#include "LevelManager.h"
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

    class LevelManagerService final : public dae::IPlatformService
    {
    public:
        void Init() override {}
        void Update() override { Tron::LevelManager::GetInstance().Update(); }
        void Shutdown() override {}
    };

    class BulletPoolService final : public dae::IMemoryPoolService
    {
    public:
        void Update() override { Tron::BulletManager::GetInstance().Update(); }
        void FireWeapon(glm::vec3 startPos, glm::vec2 velocity, int teamID, dae::GameObject* shooter) override
        {
            Tron::Team actualTeam = static_cast<Tron::Team>(teamID);

            Tron::BulletManager::GetInstance().FireBullet(startPos, velocity, actualTeam, shooter);
        }

        void Render() const override { Tron::BulletManager::GetInstance().Render(); }

        void ClearAll() override { Tron::BulletManager::GetInstance().ClearAll(); }
    };
}
#endif