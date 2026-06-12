#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
    class Scene;

    /// @brief Singleton that owns all Scenes and drives the active one each frame.
    ///
    /// Scenes are created with CreateScene() and stored in order of creation.
    /// Only one scene is "active" at a time; the active scene receives Update(),
    /// Render(), and RenderUI() calls. Switch the active scene with SetActiveScene()
    /// using the index returned implicitly by creation order.
    ///
    /// Typical lifecycle:
    /// @code
    ///   auto& scene = SceneManager::GetInstance().CreateScene();  // index 0
    ///   // ... populate scene ...
    ///   SceneManager::GetInstance().SetActiveScene(0);
    /// @endcode
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        /// @brief Creates a new empty Scene, appends it to the scene list, and returns a reference.
        /// @return Reference to the newly created Scene.
        Scene& CreateScene();

        /// @brief Returns a reference to the currently active Scene.
        Scene& GetActiveScene();

        /// @brief Makes the scene at @p index the active scene.
        /// @param index Zero-based index into the internal scene list.
        void SetActiveScene(size_t index);

        /// @brief Destroys all scenes and resets the active scene index to 0.
        void ClearScenes();

        /// @brief Forwards Update() to the active scene.
        void Update();

        /// @brief Forwards Render() to the active scene.
        void Render();

        /// @brief Forwards RenderUI() to the active scene.
        void RenderUI();

        /// @brief Returns the total number of scenes currently registered.
        size_t GetSceneCount() const { return m_scenes.size(); }

        /// @brief Returns the zero-based index of the currently active scene.
        size_t GetCurrentSceneIDX() const { return m_ActiveSceneIndex; }

        /// @brief Resets the active scene index to 0 without destroying any scenes.
        void ResetSceneIDX() { m_ActiveSceneIndex = 0; }

    private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;

        std::vector<std::unique_ptr<Scene>> m_scenes{};    ///< Ordered list of all registered scenes.
        size_t m_ActiveSceneIndex{ 0 };                    ///< Index of the scene currently receiving updates.
    };

} // namespace dae