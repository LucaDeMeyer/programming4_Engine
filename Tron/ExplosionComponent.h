#ifndef EXPLOSION_COMP_H
#define EXPLOSION_COMP_H
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class SpriteComponent;
}

namespace Tron
{
	class ExplosionComponent : public dae::BaseComponent , public dae::Observer
	{
	public:
		explicit ExplosionComponent(dae::GameObject* owner);

		~ExplosionComponent() override = default;

		ExplosionComponent(const ExplosionComponent& other) = delete;
		ExplosionComponent(ExplosionComponent&& other) = delete;
		ExplosionComponent& operator=(ExplosionComponent&& other) = delete;
		ExplosionComponent& operator=(const ExplosionComponent& other) = delete;

		void Update() override;
		void Render() const override;


		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;

	private:
		std::unique_ptr<dae::SpriteComponent> m_Sprite;
		bool m_Play = false;
		bool m_WasDestruction{ false };
	};
}

#endif
