#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H
#include <functional>

#include "BaseComponent.h"

namespace dae
{
	class ButtonComponent : public BaseComponent
	{
	public:
		explicit ButtonComponent(GameObject* owner) : BaseComponent(owner){}
	

		~ButtonComponent() override = default;
		ButtonComponent(const ButtonComponent& other) = delete;
		ButtonComponent(ButtonComponent&& other) noexcept = delete;
		ButtonComponent& operator=(const ButtonComponent& other) = delete;
		ButtonComponent& operator=(ButtonComponent&& other) noexcept = delete;

		using ClickCallback = std::function<void()>;
		void SetCallback(ClickCallback callback) { m_Callback = std::move(callback); }
		void OnClick() { if (m_Callback) m_Callback(); }


		 void Update() override{}
		 void Render() const override{}    
	private:
		ClickCallback m_Callback;
	};
}

#endif
