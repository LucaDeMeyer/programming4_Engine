#include "ScoreDisplay.h"

#include <iostream>


#include "GameObject.h"
#include "TextComponent.h"
#include "TronEvents.h"

Tron::ScoreDisplay::ScoreDisplay(dae::GameObject* owner): BaseComponent(owner), Observer()
{
	m_Text = GetOwner()->GetComponent<dae::TextComponent>();
    m_Text->SetFont("TRON.TTF", 15);
	m_Text->SetText("Score:" + std::to_string(m_CurrentScore));
    m_Text->SetColor(255, 255, 255, 255);
}


void Tron::ScoreDisplay::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (event.ID == dae::make_sdbm_hash("ScoreChangedEvent"))
    {
        if (auto* payload = static_cast<ScoreGainedARGS*>(event.pArgs.get()))
        {
            m_CurrentScore = payload->points;
            m_Text->SetText("Score: " + std::to_string(m_CurrentScore));
        }
    }
}
