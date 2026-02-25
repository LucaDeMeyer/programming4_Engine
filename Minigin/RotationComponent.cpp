#include "RotationComponent.h"

#include <iostream>

#include "GameTime.h"
#include "TransformComponent.h"
#include "GameObject.h"

void dae::RotationComponent::Update()
{
    auto transform = GetOwner()->GetTransform();
    if (!transform) return;

    float currentRot = transform->GetLocalRotation();
    float delta = m_RotationSpeed * Time::GetInstance().GetDeltaTime();
    float newRot = currentRot + delta;

    transform->SetLocalRotation(newRot);
}
