#include "GameManager.h"

#include "EventQueue.h"
#include "GameActorComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "TronEvents.h"
#include "TransformComponent.h"
void Tron::GameManager::Init()
{
    dae::EventQueue::GetInstance().GetNotifier()->AddObserver(&Tron::GameManager::GetInstance());
}

void Tron::GameManager::OnNotify(dae::GameObject* pEntity, const dae::Event& event)
{
    if (event.ID == dae::make_sdbm_hash("ActorDied"))
    {
        auto* data = static_cast<ActorDied*>(event.pArgs.get());

        if (data && data->obj)
        {
            RemoveEntity(data->obj);

            dae::Event winEvent(dae::make_sdbm_hash("WinCondition"));
            dae::EventQueue::GetInstance().AddEvent(std::move(winEvent));
        }
    }

    if (event.ID == dae::make_sdbm_hash("Teleport"))
    {
        auto* data = static_cast<Teleport*>(event.pArgs.get());
        if (data && data->obj)
        {
            glm::vec3 newPos = LevelManager::GetInstance().GetRandomPathLocation();
            auto* transform = data->obj->GetTransform();

            transform->SetLocalPosition(newPos);
            transform->SetPreviousPosition(newPos); 
        }
    }

    if (event.ID == dae::make_sdbm_hash("WinCondition"))
    {
        CheckWinCondition();
    }
}

void Tron::GameManager::RemoveEntity(dae::GameObject* entity)
{
	m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
    dae::InputManager::GetInstance().RemoveCommandsForObject(entity);
	entity->MarkForDestruction();

}

void Tron::GameManager::RegisterEntiy(dae::GameObject* entity)
{
    if (!entity) return;

	m_Entities.emplace_back(entity);
    entity->GetComponent<GameActor>()->GetEventSubject().AddObserver(this);
}

void Tron::GameManager::CheckWinCondition()
{
    if (m_Entities.empty())
    {
        switch (m_CurrentMode)
        {
        case GameMode::singlePlayer:
        case GameMode::COOP:
            break;
        case GameMode::PVP:
            break;
        }
    }
}



