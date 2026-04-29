#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H
#include "BaseComponent.h"
#include "CollisionManager.h"
#include "glm/vec4.hpp"

namespace dae
{

	class ColliderComponent : public BaseComponent
	{

	public: 
		explicit ColliderComponent(GameObject* owner) : BaseComponent(owner)
		{
			dae::CollisionManager::GetInstance().AddCollider(this);
		}

		~ColliderComponent() override
		{ dae::CollisionManager::GetInstance().RemoveCollider(this); }

		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) noexcept = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) noexcept = delete;

		virtual bool IsOverlapping(ColliderComponent* other) const = 0;
	};


	class BoxColliderComponent : public ColliderComponent
	{
	public:
		explicit BoxColliderComponent(GameObject* owner,glm::vec4 box) : ColliderComponent(owner) , m_LocalBox(box)
		{}
		~BoxColliderComponent() override = default;

		BoxColliderComponent(const BoxColliderComponent& other) = delete;
		BoxColliderComponent(BoxColliderComponent&& other) = delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other) = delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) = delete;

		void Update() override {}
		void Render() const override;

		glm::vec4 GetWorldBox() const;
		bool IsOverlapping(ColliderComponent* other) const override;

	private:
		glm::vec4 m_LocalBox;
	};
}

#endif
