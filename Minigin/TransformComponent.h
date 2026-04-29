#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BaseComponent.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace dae
{
	class TransformComponent : public dae::BaseComponent
	{
	public:
		TransformComponent(GameObject* owner) : BaseComponent(owner) {}
		~TransformComponent() override = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept = delete;


		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalRotation(float angle);
		void SetPreviousPosition(const glm::vec3& pos);

		glm::vec3& GetLocalPosition() { return m_localPosition; }
		glm::vec3& GetWorldPosition();
		const glm::mat4& GetWorldMatrix();
		float& GetLocalRotation() { return m_localRotation; }
		glm::vec3& GetPreviousPosition() { return m_PreviousPos; }

		void Update() override{}
		void Render() const override{}

		void SetPositionDirty();

	private:

		glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_worldPosition{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_PreviousPos{ 0,0,0 };
		glm::mat4 m_worldMatrix{ 1.0f };
		float m_localRotation{ 0.0f };                
		bool m_IsDirty = true;
	};
}
#endif