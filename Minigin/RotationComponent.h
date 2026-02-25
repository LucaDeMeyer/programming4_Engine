#ifndef ROTATION_COMPONENT_H
#define ROTATION_COMPONENT_H

#include "BaseComponent.h"
namespace dae
{
	class RotationComponent : public BaseComponent
	{
	public:

		RotationComponent(GameObject* parent) : BaseComponent(parent), m_RotationSpeed(){}

		~RotationComponent() override{};
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;

		void SetRotationSpeed(const float newSpeed) { m_RotationSpeed = newSpeed; }
		float& GetRotationSpeed()  { return m_RotationSpeed; }

		void Update() override;
		void Render() const override{}

	private:
		float m_RotationSpeed = 0;
	};
}
#endif
