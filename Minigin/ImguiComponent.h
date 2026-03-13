#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H
#include "BaseComponent.h"

namespace dae
{
	class ImguiComponent : public BaseComponent
	{
	public:

		explicit ImguiComponent(GameObject* owner) : BaseComponent(owner)
		{
		}

		~ImguiComponent() override = default;

		ImguiComponent(const ImguiComponent& other) = delete;
		ImguiComponent(ImguiComponent&& other) = delete;
		ImguiComponent& operator=(ImguiComponent&& other) = delete;
		ImguiComponent& operator=(const ImguiComponent& other) = delete;

		void Update() override {}
		void Render() const override final{}

		void RenderUI() override = 0;
	};
}
#endif
