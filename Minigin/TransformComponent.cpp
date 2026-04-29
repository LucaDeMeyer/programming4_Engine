#include "TransformComponent.h"
#include "GameObject.h"
#include "glm/trigonometric.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"

void dae::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}


void dae::TransformComponent::SetPositionDirty()
{
	if (m_IsDirty) return;

	m_IsDirty = true;
	for (auto* child : m_pOwner->GetChildren())
	{
		child->GetTransform()->SetPositionDirty();
	}

}

void dae::TransformComponent::SetPreviousPosition(const glm::vec3& pos)
{
	m_PreviousPos = pos;
}


void dae::TransformComponent::SetLocalRotation(float angle)
{
	m_localRotation = angle;
	SetPositionDirty(); 
}

 glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	GetWorldMatrix();

	m_worldPosition = glm::vec3(m_worldMatrix[3]);
	return m_worldPosition;
}

const glm::mat4& dae::TransformComponent::GetWorldMatrix()
{
	if (m_IsDirty)
	{
		glm::mat4 localMatrix = glm::mat4(1.0f);

		localMatrix = glm::translate(localMatrix, m_localPosition);
		localMatrix = glm::rotate(localMatrix, glm::radians(m_localRotation), glm::vec3(0, 0, 1));

		GameObject* parent = GetOwner()->GetParent();
		if (parent)
		{
		
			m_worldMatrix = parent->GetTransform()->GetWorldMatrix() * localMatrix;
		}
		else
		{
			m_worldMatrix = localMatrix;
		}

		m_IsDirty = false;
	}
	return m_worldMatrix;
	
}
