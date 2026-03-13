#include "LivesDisplay.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TronEvents.h" 

void LivesDisplay::SetTexture(const std::string& filename)
{
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void LivesDisplay::Render() const
{
    if (!m_Texture || m_Lives <= 0) return;

    auto transform = GetOwner()->GetTransform();
    if (transform)
    {
        const auto& pos = transform->GetWorldPosition();
        for (int i = 0; i < m_Lives; ++i)
        {
            float xOffset = pos.x + (i * m_Spacing);
            dae::Renderer::GetInstance().RenderTexture(*m_Texture, xOffset, pos.y);
        }
    }
}

void LivesDisplay::OnNotify(dae::GameObject* obj, const dae::Event& event)
{
    if (const auto* livesEvent = dynamic_cast<const LivesChanged*>(&event))
    {
        m_Lives = livesEvent->currentLives;
    }
}