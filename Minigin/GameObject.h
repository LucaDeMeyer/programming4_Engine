#pragma once
#include <string>
#include <memory>

#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class BaseComponent;
	class GameObject final
	{
	public:

		virtual void Update();
		virtual void Render() const;

		GameObject() = default;
		virtual ~GameObject();
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
				if (dynamic_cast<T>(comp.get()))
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
	private:
		std::vector < std::unique_ptr<BaseComponent>> m_Components;
		bool m_MarkedForDestruction = false;
	};
}
