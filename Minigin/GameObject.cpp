#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
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
			m_transform->SetLocalPosition(m_transform->GetWorldPosition() - parent->GetComponent<TransformComponent>()->GetWorldPosition());
		}
		m_transform->SetPositionDirty();
	}
	if (m_pParent) m_pParent->removeChild(this);
	m_pParent = parent;
	if (m_pParent)
	{
		auto childPtr = m_pParent->removeChild(this);
		parent->AddChild(std::move(childPtr));
	}
}

void dae::GameObject::AddChild(std::unique_ptr<GameObject> child)
{
	child->m_pParent = this;
	m_Children.emplace_back(std::move(child));
}

bool dae::GameObject::IsChild(GameObject* obj) // do we only need to check at first level or should we go over all descendants?
{
	return std::find_if(m_Children.begin(),m_Children.end(),[obj](const std::unique_ptr<GameObject>& child)
	{
			return child.get() == obj;
		}) != m_Children.end();
}

std::unique_ptr<dae::GameObject> dae::GameObject::removeChild(GameObject* child)
{
	auto it = std::find_if(m_Children.begin(), m_Children.end(),
		[child](const auto& p) { return p.get() == child; });

	if (it != m_Children.end()) {
		std::unique_ptr<GameObject> releasedChild = std::move(*it);
		m_Children.erase(it);


		releasedChild->m_pParent = nullptr;
		return releasedChild;
	}
	return nullptr;
}


const std::vector<std::unique_ptr<dae::GameObject>>& dae::GameObject::GetChildren() const
{
	return m_Children;
}