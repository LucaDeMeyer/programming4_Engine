#ifndef FPS_COMPONENT_H
#define FPS_COMPONENT_H
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
	class FPSComponent : public BaseComponent
	{
	public:
		explicit FPSComponent(GameObject* owner) : BaseComponent(owner)
		{
		}

		~FPSComponent() override = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) noexcept = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) noexcept = delete;

		void Update() override;
		void Render() const override{}
		
	private:
		float m_elapsedTime = 0.0f;
		int m_frameCount = 0;
		TextComponent* m_TextComp{ nullptr };
	};
}
#endif
