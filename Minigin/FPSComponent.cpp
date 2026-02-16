
#include <iomanip>
#include <sstream>
#include "FPSComponent.h"
#include "Font.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameTime.h"

void dae::FPSComponent::Update()
{
    if (m_TextComp == nullptr)
        m_TextComp = GetParent()->GetComponent<TextComponent>();

    m_frameCount++;

    m_elapsedTime += Time::GetInstance().GetDeltaTime();

    if (m_elapsedTime >= 1.0f)
    {
        float fps = static_cast<float>(m_frameCount) / m_elapsedTime;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << fps << "FPS";

        if (m_TextComp)
            m_TextComp->SetText(stream.str());

        m_frameCount = 0;
        m_elapsedTime = 0.0f; 
    }
}

