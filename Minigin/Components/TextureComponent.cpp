#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TransformComponent.h"

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

dae::Texture2D* dae::TextureComponent::GetTexture() const
{
	return m_Texture.get();
}


void dae::TextureComponent::Render() const
{
    if (!m_Texture) return;

    auto transform = GetOwner()->GetTransform();
    const auto& pos = transform ? transform->GetWorldPosition() : glm::vec3{ 0, 0, 0 };

    if (m_UseCustomSize)
    {
        Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_Width, m_Height);
    }
    else if (m_Scale != 1.0f)
    {
        float w, h;
        SDL_GetTextureSize(m_Texture->GetSDLTexture(), &w, &h);
        Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, w * m_Scale, h * m_Scale);
    }
    else
    {
        // Use the original overload (natural size)
        Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
    }
}
