#ifndef SCORE_DISPLAY_H
#define SCORE_DISPLAY_H
#include "BaseComponent.h"
#include "Observer.h"
#include "EventQueue.h"

namespace dae
{
	class TextComponent;
}
namespace Tron
{
	class ScoreDisplay : public dae::BaseComponent, public dae::Observer
	{
	public:
		explicit ScoreDisplay(dae::GameObject* owner);
		~ScoreDisplay() override = default;

		ScoreDisplay(const ScoreDisplay& other) = delete;
		ScoreDisplay(ScoreDisplay&& other) = delete;
		ScoreDisplay& operator=(ScoreDisplay&& other) = delete;
		ScoreDisplay& operator=(const ScoreDisplay& other) = delete;

		void Update() override {}
		void Render() const override{};

		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;
	private:
		dae::TextComponent* m_Text;
		int m_CurrentScore{0};
	};
}
#endif
