#ifndef BULLET_OBSERVER
#define BULLET_OBSERVER
#include "Observer.h"
#include "BaseComponent.h"
#include "EventQueue.h"

namespace Tron
{
	class BulletObserver : public dae::BaseComponent, public dae::Observer
	{
	public:
		explicit BulletObserver(dae::GameObject* owner) : BaseComponent(owner), Observer()
		{
			dae::EventQueue::GetInstance().GetNotifier()->AddObserver(this);
		}

		~BulletObserver() override
		{
			dae::EventQueue::GetInstance().GetNotifier()->RemoveObserver(this);
		}

		BulletObserver(const BulletObserver& other) = delete;
		BulletObserver(BulletObserver&& other) = delete;
		BulletObserver& operator=(BulletObserver&& other) = delete;
		BulletObserver& operator=(const BulletObserver& other) = delete;

		void Update() override {}
		void Render() const override {}

		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;
	};
}
#endif
