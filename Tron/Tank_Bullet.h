#ifndef TANK_BULLET_H
#define TANK_BULLET_H
#include <memory>
#include <string>

#include "BaseComponent.h"
#include "glm/vec2.hpp"
#include "TextureComponent.h"
namespace Tron
{
	class TankBullet : public dae::BaseComponent
	{
	public:
		explicit TankBullet(dae::GameObject* owner, glm::vec2 velocity);
		~TankBullet() override = default;
		TankBullet(const TankBullet& other) = delete;
		TankBullet(TankBullet&& other) noexcept = delete;
		TankBullet& operator=(const TankBullet& other) = delete;
		TankBullet& operator=(TankBullet&& other) noexcept = delete;

		void Update() override;
		void Render() const override{}

	private:
		glm::vec2 m_Velocity;
	};
}

#endif
