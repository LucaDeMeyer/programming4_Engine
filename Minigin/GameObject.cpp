#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "TextureComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& comp  : m_Components)
	{
		comp->Update();
	}
}


void dae::GameObject::Render() const
{
	for (const auto& comp : m_Components)
	{
		comp->Render();
	}
}

void dae::GameObject::LateUpdate()
{
	std::erase_if(m_Components, [](const auto& comp) {
		return comp->IsMarkedForDestruction();
		});
}

