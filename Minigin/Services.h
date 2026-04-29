#ifndef SERVICES_H
#define SERVICES_H
#include <string>
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

    enum class AudioType
    {
        Ambient,
        FX
    };

    class IAudioService
    {
    public:

       

    	virtual ~IAudioService() = default;
        virtual bool Init() = 0;
        virtual void Play(unsigned int soundHash,float volume, AudioType type) = 0;
        virtual void Pause(unsigned int soundHash, AudioType type) = 0;
        virtual void Stop(unsigned int soundHash, AudioType type) = 0;
        virtual void StopAll() = 0;
        virtual void LoadSound(unsigned int soundHash, const std::string& filepath) = 0;

    };

    class NullAudioService : public IAudioService
    {
    public:
        bool Init() override { return true; }
        void Play(unsigned int ,float,AudioType ) override{}
        void Pause(unsigned int,AudioType ) override {}
        void Stop(unsigned int, AudioType) override {}
        void StopAll() override{}
        void LoadSound(unsigned int , const std::string& ) override{}
    };
}

#endif