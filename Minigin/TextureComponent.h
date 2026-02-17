#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H

#include <memory>
#include <string>

#include "BaseComponent.h"
#include "Texture2D.h"


namespace dae
{
	class TextureComponent : public BaseComponent
	{
	public:
		TextureComponent(GameObject* owner) : BaseComponent(owner) {}

		~TextureComponent() override = default;

		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) noexcept = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

		void Update() override{}
		void Render() const override;

		dae::Texture2D* GetTexture() const;
		void SetTexture(const std::string& filename);
		
	private:
		std::shared_ptr<Texture2D> m_Texture;

	};
}
#endif