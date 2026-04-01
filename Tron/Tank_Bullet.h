#ifndef TANK_BULLET_H
#define TANK_BULLET_H
#include <memory>
#include <string>

#include "BaseComponent.h"
#include "glm/vec2.hpp"
#include "TextureComponent.h"
#include "glm/vec4.hpp"

namespace Tron
{
	class TankBullet : public dae::BaseComponent
	{
	public:
		explicit TankBullet(dae::GameObject* owner, dae::GameObject* shooter, glm::vec2 velocity);
		~TankBullet() override = default;
		TankBullet(const TankBullet& other) = delete;
		TankBullet(TankBullet&& other) noexcept = delete;
		TankBullet& operator=(const TankBullet& other) = delete;
		TankBullet& operator=(TankBullet&& other) noexcept = delete;

		void Update() override;
		void Render() const override{}

		dae::GameObject* GetShooter() const { return m_Shooter; }
		void Bounce(const glm::vec4& wallBox, const glm::vec4& bulletBox);
		int m_MaxnrBounces{ 3 };
	private:
		dae::GameObject* m_Shooter{ nullptr };
		glm::vec2 m_Velocity;
	
	};
}

#endif
