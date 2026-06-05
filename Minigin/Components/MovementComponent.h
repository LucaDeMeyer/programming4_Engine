#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H
#include "BaseComponent.h"
namespace dae
{
	class MovementComponent : public BaseComponent
	{
	public:
		explicit MovementComponent(GameObject* owner) :  BaseComponent(owner){}
		~MovementComponent() override = default;

		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;

		void Update() override{}
		void Render() const override{}

		void SetMovementSpeed(float newSpeed) { m_Speed = newSpeed; }

		void MoveVertical(int direction);
		void MoveHorizontal(int direction);

	private:
		float m_Speed = 100.f;
	};

}
#endif
