#ifndef TANK_BULLET_H
#define TANK_BULLET_H
#include <memory>
#include "BaseComponent.h"

class TankBullet : public dae::BaseComponent
{
	explicit TankBullet(dae::GameObject* owner) : BaseComponent(owner)
	{
	}
	~TankBullet() override = default;
	TankBullet(const TankBullet& other) = delete;
	TankBullet(TankBullet&& other) noexcept = delete;
	TankBullet& operator=(const TankBullet& other) = delete;
	TankBullet& operator=(TankBullet&& other) noexcept = delete;

	void Update() override{}
	void Render() const override{}
};

#endif
