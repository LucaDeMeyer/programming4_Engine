#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H
#include "BaseComponent.h"

namespace dae
{
	class UIComponent : public BaseComponent
	{
	public:

		explicit UIComponent(GameObject* owner) : BaseComponent(owner)
		{
		}

		~UIComponent() override = default;

		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;

		void Update() override {}
		void Render() const override final{}

		void RenderUI() override = 0;

	
	};
}
#endif
