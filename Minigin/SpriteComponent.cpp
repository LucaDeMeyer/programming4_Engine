#include "SpriteComponent.h"
#include "Renderer.h"
#include "SDL3/SDL_rect.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include"TransformComponent.h"

dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, int cols, int rows)
	:BaseComponent(owner),m_Cols(cols),m_Rows(rows)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
    if (m_Texture)
    {
        auto size = m_Texture->GetSize(); 
        m_FrameWidth = static_cast<float>(size.x) / m_Cols;
        m_FrameHeight = static_cast<float>(size.y) / m_Rows;
    }
}

void dae::SpriteComponent::Render() const
{
    if (!m_Texture) return;

    int col = m_CurrentFrame % m_Cols;
    int row = m_CurrentFrame / m_Cols;

    float srcX = col * m_FrameWidth;
    float srcY = row * m_FrameHeight;

    auto pos = GetOwner()->GetTransform()->GetWorldPosition();

    Renderer::GetInstance().RenderTexture(*m_Texture,pos.x, pos.y,srcX, srcY, m_FrameWidth, m_FrameHeight);
}
