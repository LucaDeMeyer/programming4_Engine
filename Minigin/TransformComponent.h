#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "BaseComponent.h"
#include "glm/vec3.hpp"

namespace dae
{
	class TransformComponent : public dae::BaseComponent
	{
	public:
		TransformComponent(GameObject* owner) : BaseComponent(owner) , m_position(){}
		~TransformComponent() override = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) noexcept = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) noexcept = delete;

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);

		void Update() override{}
		void Render() const override{}

	private:
		glm::vec3 m_position;
	};
}
#endif