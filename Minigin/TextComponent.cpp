#include "TextComponent.h"
#include <stdexcept>
#include "Renderer.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "Font.h"
dae::TextComponent::TextComponent(dae::GameObject* parent) : BaseComponent(parent)
{
}

dae::TextComponent::~TextComponent() = default;

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		if (m_text.empty() || m_font == nullptr)
		{
			return;
		}

		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_textTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_textTexture != nullptr)
	{
		if (const auto transformComponent{ m_pOwner->GetTransform() })
		{
			const auto pos = transformComponent->GetWorldPosition();
			Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
		}
	}
}

dae::TextComponent* dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	if (m_font) m_needsUpdate = true;
	return this;
}

std::string dae::TextComponent::GetText() const
{
	return m_text;
}
dae::TextComponent* dae::TextComponent::SetOffset(const glm::vec2& offset)
{
	m_Offset = offset;
	return this;
}
glm::vec2 dae::TextComponent::GetOffset() const
{
	return m_Offset;
}
dae::TextComponent* dae::TextComponent::SetFont(const std::string& fontPath, Uint8 fontSize)
{
	m_font = ResourceManager::GetInstance().LoadFont(fontPath, fontSize);
	m_needsUpdate = true;
	return this;
}

dae::TextComponent* dae::TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	m_color = SDL_Color{ r, g, b, a };
	m_needsUpdate = true;
	return this;
}

SDL_Color dae::TextComponent::GetColor()
{
	return m_color;
}

