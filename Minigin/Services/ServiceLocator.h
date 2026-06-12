#ifndef SERVICE_LOCATOR_H
#define SERVICE_LOCATOR_H
#include <memory>
#include "Services.h"

namespace dae
{
    /// @brief Global registry for engine-wide service interfaces.
    ///
    /// Implements the Service Locator pattern. Each service type has a static
    /// getter and a registration method. If no concrete service is registered,
    /// a Null Object implementation is returned so callers never need to
    /// null-check the result.
    ///
    /// Typical usage:
    /// @code
    ///   // Registration (once, at startup):
    ///   ServiceLocator::RegisterAudioService(std::make_unique<AudioService>());
    ///
    ///   // Consumption (anywhere):
    ///   ServiceLocator::GetAudioService().Play(hash, 1.f, AudioType::FX);
    /// @endcode
    ///
    /// @note This class is not instantiable; all members are static.
    class ServiceLocator
    {
    public:
        // -----------------------------------------------------------------
        // Achievement Service
        // -----------------------------------------------------------------

        /// @brief Returns the active achievement service.
        static IAchievementService& GetAchievements() { return *m_Achievements; }

        /// @brief Registers a concrete achievement service.
        ///        Passing nullptr installs the NullAchievementService.
        /// @param service Owning pointer to the service implementation.
        static void RegisterAchievements(std::unique_ptr<IAchievementService> service) {
            m_Achievements = service ? std::move(service)
                : std::make_unique<NullAchievementService>();
        }

        // -----------------------------------------------------------------
        // Platform Service
        // -----------------------------------------------------------------

        /// @brief Returns the active platform service.
        static IPlatformService& GetPlatform() { return *m_Platform; }

        /// @brief Registers a concrete platform service.
        ///        Passing nullptr installs the NullPlatformService.
        /// @param service Owning pointer to the service implementation.
        static void RegisterPlatform(std::unique_ptr<IPlatformService> service) {
            m_Platform = service ? std::move(service)
                : std::make_unique<NullPlatformService>();
        }

        // -----------------------------------------------------------------
        // Audio Service
        // -----------------------------------------------------------------

        /// @brief Returns the active audio service.
        static IAudioService& GetAudioService() { return *m_AudioService; }

        /// @brief Registers a concrete audio service.
        ///        Passing nullptr installs the NullAudioService.
        /// @param service Owning pointer to the service implementation.
        static void RegisterAudioService(std::unique_ptr<IAudioService> service) {
            m_AudioService = service ? std::move(service)
                : std::make_unique<NullAudioService>();
        }

        // -----------------------------------------------------------------
        // Game Service
        // -----------------------------------------------------------------

        /// @brief Returns the active game service.
        static IGameService& GetGameService() { return *m_GameService; }

        /// @brief Registers a concrete game service.
        ///        Passing nullptr installs the NullGameService.
        /// @param service Owning pointer to the service implementation.
        static void RegisterGameService(std::unique_ptr<IGameService> service) {
            m_GameService = service ? std::move(service)
                : std::make_unique<NullGameService>();
        }

    private:
        ServiceLocator() = delete;

        static inline std::unique_ptr<IAchievementService> m_Achievements
            = std::make_unique<NullAchievementService>();
        static inline std::unique_ptr<IPlatformService>    m_Platform
            = std::make_unique<NullPlatformService>();
        static inline std::unique_ptr<IAudioService>       m_AudioService
            = std::make_unique<NullAudioService>();
        static inline std::unique_ptr<IGameService>        m_GameService
            = std::make_unique<NullGameService>();
    };

} // namespace dae
#endif