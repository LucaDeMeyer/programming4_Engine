#include "TransformComponent.h"
#include "GameObject.h"
void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::TransformComponent::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}


void dae::TransformComponent::SetPositionDirty()
{
	if (m_IsDirty) return;

	m_IsDirty = true;

	for (auto& child : m_pOwner->GetChildren()) {
		auto childTransform = child->GetComponent<TransformComponent>();
		if (childTransform) childTransform->SetPositionDirty();
	}

}

void dae::TransformComponent::SetLocalRotation(float angle)
{
	m_localRotation = angle;
	SetPositionDirty(); 
}

glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		GameObject* parent = GetParent()->GetParent();

		if (parent)
		{
			m_worldPosition = parent->GetComponent<dae::TransformComponent>()->GetWorldPosition() + m_localPosition;
		}
		else
		{
			m_worldPosition = m_localPosition;
		}
		m_IsDirty = false;
		
	}
	return m_worldPosition;
}
