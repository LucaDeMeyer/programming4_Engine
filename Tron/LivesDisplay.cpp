#include "LivesDisplay.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TronEvents.h" 
#include "Utils.h"
using namespace Tron;
void LivesDisplay::SetTexture(const std::string& filename)
{
    m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void LivesDisplay::Render() const
{
    if (!m_Texture) return;

    if (m_Lives <= 0) return;

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
    if (event.ID == dae::Utils::make_sdbm_hash("LivesChangedEvent"))
    {
        if (auto* payload = static_cast<LivesChangedARGS*>(event.pArgs.get()))
        {
            m_Lives = payload->currentLives;
        }
    }

    if (event.ID == dae::Utils::make_sdbm_hash("PlayerDiedEvent"))
    {
	    if (auto* payload = static_cast<PlayerDiedARGS*>(event.pArgs.get()))
	    {
            m_Lives = 0;
	    }
    }
}