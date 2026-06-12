#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <string>
#include <memory>
#include <vector>

#include "BaseComponent.h"
namespace dae
{
    class Texture2D;
    class BaseComponent;
    class TransformComponent;

    /**
     * @brief The base container for all entities in the scene.
     * Manages components, lifecycle, and parent-child scene graph relationships.
     */
    class GameObject final
    {
    public:
        /// @brief Updates all components attached to this object.
        void Update();

        /// @brief Renders all components attached to this object.
        void Render() const;

        /// @brief Renders UI-specific elements for all attached components.
        void RenderUI();

        /// @brief Executes end-of-frame logic, such as cleaning up components marked for destruction.
        void LateUpdate();

        GameObject();
        ~GameObject();
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        /**
         * @brief Adds a component of type T to the object.
         * @tparam T The component type to add.
         * @tparam Args Argument types for the component constructor.
         * @param args Arguments forwarded to the component constructor.
         * @return A raw pointer to the created component.
         */
        template<typename T, typename... Args>
        T* AddComponent(Args&&...args)
        {
            std::unique_ptr<T> comp = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* rawPtr = comp.get();
            m_Components.push_back(std::move(comp));
            return rawPtr;
        }

        /**
         * @brief Retrieves the first component of type T found on this object.
         * @tparam T The component type to search for.
         * @return A pointer to the component, or nullptr if not found.
         */
        template<typename T>
        T* GetComponent() const
        {
            for (auto& comp : m_Components)
            {
                if (auto ptr = dynamic_cast<T*>(comp.get()))
                    return ptr;
            }
            return nullptr;
        }

        /// @brief Checks if a component of type T exists on this object.
        template <typename T>
        bool HasComponent() const { return GetComponent<T>() != nullptr; }

        /// @brief Marks the first component of type T for destruction.
        template<typename T>
        void RemoveComponent()
        {
            for (auto& comp : m_Components)
            {
                if (dynamic_cast<T*>(comp.get()))
                {
                    comp->MarkForDestruction();
                    return;
                }
            }
        }

        /// @brief Marks this object and all its children for destruction at the end of the frame.
		void MarkForDestruction()
		{
			m_MarkedForDestruction = true;
 			for (auto child : m_Children)
				child->MarkForDestruction();
		}
        /// @brief Returns true if this object is scheduled for removal from the scene.
		bool IsMarkedForDestruction() { return m_MarkedForDestruction; }

		/// @brief Sets the parent of this GameObject.
		/// @param parent The new parent object, or nullptr to detach from the current parent.
		/// @param keepWorldPos If true, adjusts the local position to maintain the same world-space coordinate.
		void SetParent(GameObject* parent, bool keepWorldPos);

        // @brief Returns a reference to the list of child GameObjects.
        std::vector<dae::GameObject*>& GetChildren();

        /// @brief Returns the parent of this GameObject, or nullptr if it is a root object.
        GameObject* GetParent() const { return m_pParent; }

        /// @brief Returns the Transform component, which manages position, rotation, and scale.
        TransformComponent* GetTransform() { return m_transform.get(); }

	private:

		void removeChild(GameObject* child);
		void AddChild(GameObject* child);
		bool IsChild(GameObject* obj) const;

		GameObject* m_pParent = nullptr;
		std::vector <GameObject*> m_Children;

		std::vector< std::unique_ptr<BaseComponent>> m_Components;
		std::unique_ptr<TransformComponent> m_transform;
		bool m_MarkedForDestruction = false;
	};
}
#endif 