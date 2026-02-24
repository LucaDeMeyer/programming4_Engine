#include "RotationComponent.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include "GameObject.h"

void dae::RotationComponent::Update()
{
	float angleChange = m_RotationSpeed * Time::GetInstance().GetDeltaTime();

	auto transform = GetParent()->GetComponent<dae::TransformComponent>();
	if (!transform) return;

	glm::vec3 pivot = glm::vec3(0, 0, 0);

	GameObject* parent = GetParent()->GetParent();
	if (parent)
	{
		auto parentTransform = parent->GetComponent<dae::TransformComponent>();
		if (parentTransform)
		{
			pivot = parentTransform->GetLocalPosition();
		}
	}

	glm::vec3 pos = transform->GetLocalPosition();

	float s = sin(angleChange);
	float c = cos(angleChange);

	pos.x -= pivot.x;
	pos.y -= pivot.y;

	float xnew = pos.x * c - pos.y * s;
	float ynew = pos.x * s + pos.y * c;

	transform->SetLocalPosition(glm::vec3(xnew + pivot.x, ynew + pivot.y, pos.z));
	
}
