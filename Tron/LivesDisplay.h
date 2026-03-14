#ifndef LIVES_DISPLAY_H
#define LIVES_DISPLAY_H
#include <memory>
#include <string>
#include <vector>

#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class Texture2D;
}

namespace Tron
{
	class LivesDisplay : public dae::BaseComponent, public dae::Observer
	{
	public:
		explicit LivesDisplay(dae::GameObject* owner, int Lives) : BaseComponent(owner), Observer(), m_Lives(Lives), m_Spacing(16.0f)
		{
		}
		~LivesDisplay() override = default;

		LivesDisplay(const LivesDisplay& other) = delete;
		LivesDisplay(LivesDisplay&& other) = delete;
		LivesDisplay& operator=(LivesDisplay&& other) = delete;
		LivesDisplay& operator=(const LivesDisplay& other) = delete;

		void Update() override {};
		void Render() const override;

		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;

		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<dae::Texture2D> m_Texture;
		int m_Lives;
		float m_Spacing;
	};
}
#endif
