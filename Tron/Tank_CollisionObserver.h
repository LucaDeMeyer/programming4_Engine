#ifndef TANK_COLLISION_OBSERVER_H
#define TANK_COLLISION_OBSERVER_H
#include "Observer.h"
#include "BaseComponent.h"
#include "EventQueue.h"

namespace Tron
{
	class TankCollisionObserver : public dae::BaseComponent ,public dae::Observer
	{
	public:
		explicit TankCollisionObserver(dae::GameObject* owner) : BaseComponent(owner), Observer()
		{
			dae::EventQueue::GetInstance().GetNotifier()->AddObserver(this);
		}

		~TankCollisionObserver() override = default;

		TankCollisionObserver(const TankCollisionObserver& other) = delete;
		TankCollisionObserver(TankCollisionObserver&& other) = delete;
		TankCollisionObserver& operator=(TankCollisionObserver&& other) = delete;
		TankCollisionObserver& operator=(const TankCollisionObserver& other) = delete;

		void Update() override {}
		void Render() const override{}

		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;
	};
}
#endif
