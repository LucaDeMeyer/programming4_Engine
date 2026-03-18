#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H
#include <memory>
#include "Services.h"
namespace dae
{
    class ServiceLocator
    {
    public:
        static IAchievementService& GetAchievements() { return *m_Achievements; }
        static void RegisterAchievements(std::unique_ptr<IAchievementService> service) {
            m_Achievements = service ? std::move(service)
                : std::make_unique<NullAchievementService>();
        }

        static IPlatformService& GetPlatform() { return *m_Platform; }
        static void RegisterPlatform(std::unique_ptr<IPlatformService> service) {
            m_Platform = service ? std::move(service)
                : std::make_unique<NullPlatformService>();
        }

    private:
        ServiceLocator() = delete;

        static inline std::unique_ptr<IAchievementService> m_Achievements
            = std::make_unique<NullAchievementService>();
        static inline std::unique_ptr<IPlatformService> m_Platform
            = std::make_unique<NullPlatformService>();
    };
}
#endif