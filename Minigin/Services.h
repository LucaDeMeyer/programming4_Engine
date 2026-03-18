#ifndef SERVICES_H
#define SERVICES_H
namespace dae
{
    class IPlatformService {
    public:
        virtual ~IPlatformService() = default;
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    };

    class NullPlatformService final : public IPlatformService {
    public:
        void Init() override {}
        void Update() override {}
        void Shutdown() override {}
    };

    class IAchievementService {
    public:
        virtual ~IAchievementService() = default;
        virtual void UnlockAchievement(const char* id) = 0;
    };
    class NullAchievementService final : public IAchievementService {
    public:
        void UnlockAchievement(const char*) override {}
    };
}

#endif