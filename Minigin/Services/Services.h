#ifndef SERVICES_H
#define SERVICES_H
#include <string>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace dae
{
    class GameObject;

    // -------------------------------------------------------------------------
    // Platform Service
    // -------------------------------------------------------------------------

    /// @brief Interface for platform-level lifecycle operations.
    ///
    /// Abstracts platform-specific initialisation, per-frame updates, and
    /// shutdown so the engine core stays platform-agnostic.
    class IPlatformService {
    public:
        virtual ~IPlatformService() = default;
        /// @brief Initialises the platform layer.
        virtual void Init() = 0;
        /// @brief Performs per-frame platform updates (e.g. window events).
        virtual void Update() = 0;
        /// @brief Releases all platform resources.
        virtual void Shutdown() = 0;
    };

    /// @brief No-op implementation of IPlatformService used as a safe default.
    class NullPlatformService final : public IPlatformService {
    public:
        void Init() override {}
        void Update() override {}
        void Shutdown() override {}
    };

    // -------------------------------------------------------------------------
    // Achievement Service
    // -------------------------------------------------------------------------

    /// @brief Interface for unlocking platform achievements.
    class IAchievementService {
    public:
        virtual ~IAchievementService() = default;
        /// @brief Unlocks the achievement identified by @p id.
        /// @param id Platform-specific achievement identifier string.
        virtual void UnlockAchievement(const char* id) = 0;
    };

    /// @brief No-op implementation of IAchievementService used as a safe default.
    class NullAchievementService final : public IAchievementService {
    public:
        void UnlockAchievement(const char*) override {}
    };

    // -------------------------------------------------------------------------
    // Audio Service
    // -------------------------------------------------------------------------

    /// @brief Classifies a sound by its usage context.
    enum class AudioType
    {
        Ambient, ///< Looping background / atmospheric audio.
        FX       ///< One-shot sound effects.
    };

    /// @brief Interface for the audio system.
    ///
    /// All audio interactions go through this interface so the concrete
    /// implementation (SDL, FMOD, etc.) can be swapped or mocked freely
    /// via the ServiceLocator.
    class IAudioService
    {
    public:
        virtual ~IAudioService() = default;

        /// @brief Initialises the audio backend.
        /// @return True on success, false if initialisation failed.
        virtual bool Init() = 0;

        /// @brief Starts playback of a previously loaded sound.
        /// @param soundHash Hashed identifier of the sound asset.
        /// @param volume    Playback volume in [0.0, 1.0].
        /// @param type      Audio category (Ambient or FX).
        virtual void Play(unsigned int soundHash, float volume, AudioType type) = 0;

        /// @brief Pauses a currently playing sound.
        /// @param soundHash Hashed identifier of the sound to pause.
        /// @param type      Audio category of the sound.
        virtual void Pause(unsigned int soundHash, AudioType type) = 0;

        /// @brief Stops a currently playing sound.
        /// @param soundHash Hashed identifier of the sound to stop.
        /// @param type      Audio category of the sound.
        virtual void Stop(unsigned int soundHash, AudioType type) = 0;

        /// @brief Stops all currently playing sounds.
        virtual void StopAll() = 0;

        /// @brief Toggles global mute on/off.
        virtual void ToggleMute() = 0;

        /// @brief Loads a sound asset and associates it with a hash.
        /// @param soundHash Hashed identifier to map to this asset.
        /// @param filepath  Path to the audio file on disk.
        virtual void LoadSound(unsigned int soundHash, const std::string& filepath) = 0;
    };

    /// @brief No-op implementation of IAudioService used as a safe default.
    class NullAudioService : public IAudioService
    {
    public:
        bool Init() override { return true; }
        void Play(unsigned int, float, AudioType) override {}
        void Pause(unsigned int, AudioType) override {}
        void Stop(unsigned int, AudioType) override {}
        void StopAll() override {}
        void ToggleMute() override {}
        void LoadSound(unsigned int, const std::string&) override {}
    };

    // -------------------------------------------------------------------------
    // Game Service
    // -------------------------------------------------------------------------

    /// @brief Interface for the top-level game logic update and render pass.
    ///
    /// Allows the engine core to drive the active game mode without depending
    /// on a concrete game class.
    class IGameService {
    public:
        virtual ~IGameService() = default;
        /// @brief Advances the game logic by one tick.
        virtual void Update() = 0;
        /// @brief Renders the current game state.
        virtual void Render() const = 0;
    };

    /// @brief No-op implementation of IGameService used as a safe default.
    class NullGameService final : public IGameService {
    public:
        void Update() override {}
        void Render() const override {}
    };

} // namespace dae
#endif