#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "glm/vec2.hpp"
#include "Memory/LinearAllocator.h"

namespace dae
{
    /// @brief Engine entry point — initialises all subsystems and drives the game loop.
    ///
    /// Construct one Minigin instance at program startup, call Run() with a load
    /// callback that populates the initial scene, then let it block until the user
    /// quits. The engine is intentionally non-copyable and non-movable; only one
    /// instance should exist per process.
    ///
    /// Typical usage:
    /// @code
    ///   dae::Minigin engine{ "data/", {640, 480} };
    ///   engine.Run([]() {
    ///       auto& scene = dae::SceneManager::GetInstance().CreateScene();
    ///       // ... populate scene ...
    ///   });
    /// @endcode
    class Minigin final
    {
    public:
        /// @brief Initialises SDL, the renderer, ResourceManager, and all engine singletons.
        /// @param dataPath  Path to the asset data directory forwarded to ResourceManager::Init().
        /// @param WindowSize Desired window dimensions in pixels (width, height).
        explicit Minigin(const std::filesystem::path& dataPath, glm::vec2 WindowSize);

        /// @brief Shuts down all subsystems and closes the SDL window.
        ~Minigin();

        /// @brief Runs the load callback then enters the main loop until quit is requested.
        ///
        /// The @p load function is called once before the loop starts, giving callers
        /// a safe place to create scenes and GameObjects after all subsystems are ready.
        /// The loop calls RunOneFrame() repeatedly and exits when m_quit is set.
        ///
        /// @param load Callback invoked once to set up the initial game state.
        void Run(const std::function<void()>& load);

        /// @brief Executes exactly one engine tick: input → update → render → frame-allocator reset.
        ///
        /// Exposed publicly so host applications or tests can drive the loop manually.
        /// Under normal circumstances Run() calls this internally.
        void RunOneFrame();

        /// @brief Returns the engine-wide per-frame linear allocator (2 MiB capacity).
        ///
        /// The allocator is reset automatically at the end of each frame in RunOneFrame(),
        /// so any memory allocated here is only valid for the duration of the current tick.
        /// Use this for short-lived, frame-scoped objects (e.g. event payloads) to avoid
        /// per-frame heap allocations.
        ///
        /// @return Reference to the static LinearAllocator shared across the whole engine.
        static LinearAllocator& GetFrameAllocator() {
            static LinearAllocator frameAllocator(2 * 1024 * 1024);
            return frameAllocator;
        }

        Minigin(const Minigin&) = delete;
        Minigin(Minigin&&) = delete;
        Minigin& operator=(const Minigin&) = delete;
        Minigin& operator=(Minigin&&) = delete;

    private:
        bool m_quit{};  ///< Set to true when a quit event is received; terminates the Run() loop.
    };

} // namespace dae