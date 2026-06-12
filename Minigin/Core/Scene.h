#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae
{
    /// @brief An ordered container of GameObjects that receives engine tick and render calls.
    ///
    /// Scenes are created exclusively by SceneManager::CreateScene() (the constructor is
    /// private). Add GameObjects with Add(); remove them with Remove() or RemoveAll().
    /// Removals are deferred — objects are marked and destroyed at a safe point in the
    /// frame rather than immediately, so it is safe to call Remove() from within an
    /// Update() callback.
    ///
    /// The Scene is non-copyable and non-movable; ownership always stays with the
    /// SceneManager that created it.
    class Scene final
    {
    public:
        /// @brief Takes ownership of @p object and appends it to the scene.
        /// @param object Owning pointer to the GameObject to add.
        void Add(std::unique_ptr<GameObject> object);

        /// @brief Marks @p object for destruction at the end of the current frame.
        ///
        /// The object remains alive (and will still receive the current frame's
        /// remaining callbacks) until the deferred cleanup runs.
        ///
        /// @param object Reference to the GameObject to remove.
        // Note: not const — marking an object for deferred destruction is logically
        // a mutation of the scene even though m_objects itself is not immediately changed.
        void Remove(GameObject& object);

        /// @brief Marks all objects for destruction at the end of the current frame.
        void RemoveAll();

        /// @brief Calls Update() on all live GameObjects in insertion order.
        void Update();

        /// @brief Calls LateUpdate() on all live GameObjects after Update() has completed.
        ///
        /// Use LateUpdate() for logic that must run after all objects have been updated
        /// (e.g. camera follow, UI anchoring).
        void LateUpdate();

        /// @brief Calls Render() on all live GameObjects.
        void Render() const;

        /// @brief Calls RenderUI() on all live GameObjects.
        void RenderUI();

        ~Scene() = default;
        Scene(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene& operator=(Scene&&) = delete;

        /// @brief Returns a vector of raw (non-owning) pointers to all live GameObjects.
        ///
        /// Intended for read-only iteration (e.g. collision queries, AI line-of-sight).
        /// Do not store the returned pointers past the current frame, as objects may
        /// be destroyed by deferred removal at any time.
        ///
        /// @return A freshly built vector of raw pointers; ownership stays with the Scene.
        std::vector<GameObject*> GetObjects() {
            std::vector<GameObject*> rawObjects;
            rawObjects.reserve(m_objects.size());
            for (const auto& obj : m_objects)
                rawObjects.push_back(obj.get());
            return rawObjects;
        }

    private:
        friend class SceneManager; ///< SceneManager is the sole factory for Scene instances.
        explicit Scene() = default;

        std::vector<std::unique_ptr<GameObject>> m_objects{}; ///< Owned, ordered list of all GameObjects in this scene.
    };

} // namespace dae