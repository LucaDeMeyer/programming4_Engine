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
	if (m_Texture)
	{
		auto transform = GetParent()->GetComponent<TransformComponent>();
		if (transform)
		{
			const auto& pos = transform->GetLocalPosition();

			Renderer::GetInstance().RenderTexture(*m_Texture.get(), pos.x, pos.y);
		}
		else
			Renderer::GetInstance().RenderTexture(*m_Texture.get(), 0, 0);
	}
}
