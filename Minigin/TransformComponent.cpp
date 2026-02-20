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
