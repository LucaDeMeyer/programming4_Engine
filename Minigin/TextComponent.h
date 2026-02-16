#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H
#include <string>
#include <memory>

#include "BaseComponent.h"
#include "TextureComponent.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_surface.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		void Update() override;

		void Render() const override;

		void SetText(const std::string& text);
		std::string GetText() const;
		void SetFont(const std::string& fontPath, Uint8 fontSize);

		void SetOffset(const glm::vec2& offset);
		glm::vec2 GetOffset() const;

		TextComponent(dae::GameObject* parent);
		~TextComponent() override;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;


		SDL_Color GetColor();
		void SetColor(Uint8 r, Uint8 g, Uint8 b,Uint8 a);

	private:
		SDL_Color m_color{};
		std::string m_text{};
		std::shared_ptr<Font> m_font;
		bool m_needsUpdate{ false };
		std::shared_ptr<Texture2D> m_textTexture;
		glm::vec2 m_Offset{};
	};
}

#endif