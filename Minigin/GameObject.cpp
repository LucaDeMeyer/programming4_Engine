#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
dae::GameObject::GameObject()
{
	m_transform = std::make_unique<TransformComponent>(this);
}
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

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPos)
{
	if (m_pParent == parent || parent == this || IsChild(parent))
		return;
	if (parent == nullptr)
	{
		m_transform->SetLocalPosition(m_transform->GetWorldPosition());
	}
	else
	{
		if (keepWorldPos)
		{
			m_transform->SetLocalPosition(m_transform->GetWorldPosition() - parent->GetTransform()->GetWorldPosition());
		}
		m_transform->SetPositionDirty();
	}
	if (m_pParent) m_pParent->removeChild(this);
	m_pParent = parent;
	if (m_pParent)
		parent->AddChild(this);
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (!child || child == this)
		return;

	if (std::find(m_Children.begin(), m_Children.end(), child) != m_Children.end())
		return;

	child->m_pParent = this;
	m_Children.emplace_back(child);
}

bool dae::GameObject::IsChild(GameObject* obj) const
{
	for (auto* child : m_Children)
	{
		if (child == obj || child->IsChild(obj))
			return true;
	}
	return false;
}

void dae::GameObject::removeChild(GameObject* child)
{
	std::erase(m_Children, child);
}


std::vector<dae::GameObject*>& dae::GameObject::GetChildren()
{
	return m_Children;
}