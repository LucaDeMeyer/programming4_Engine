#pragma once
#include <string>
#include <memory>
#include <vector>

#include "BaseComponent.h"
namespace dae
{
	class Texture2D;
	class BaseComponent;

	class TransformComponent;
	class GameObject final
	{
	public:

		void Update();
		void Render() const;
		void LateUpdate();

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template<typename T,typename... Args>
		T* AddComponent(Args&&...args)
		{
			std::unique_ptr<T> comp = std::make_unique<T>(this,std::forward<Args>(args)...);
			m_Components.push_back(std::move(comp));

			return comp.get();
		}

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

		template <typename T>
		bool HasComponent() const { return GetComponent<T>() != nullptr; }

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

		void MarkForDestruction()
		{
			m_MarkedForDestruction = true;
		}

		bool IsMarkedForDestruction() { return m_MarkedForDestruction; }

		void SetParent(GameObject* parent, bool keepWorldPos);
		std::vector<dae::GameObject*>& GetChildren();
		GameObject* GetParent() const { return m_pParent; }
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
