#include "GameManager.h"

#include <iostream>

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

    if (event.ID == dae::make_sdbm_hash("WinCondition"))
    {
        CheckWinCondition();
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

 
}

void Tron::GameManager::RemoveEntity(dae::GameObject* entity)
{
    ActorType type = entity->GetComponent<GameActor>()->GetActorType();

    if (type == ActorType::player)
        m_Players -= 1;
    if (type == ActorType::enemy)
        m_enemies -= 1;

	m_Entities.erase(std::remove(m_Entities.begin(), m_Entities.end(), entity), m_Entities.end());
    dae::InputManager::GetInstance().RemoveCommandsForObject(entity);
	entity->MarkForDestruction();

}

void Tron::GameManager::RegisterEntiy(dae::GameObject* entity)
{
    if (!entity) return;

	m_Entities.emplace_back(entity);
    entity->GetComponent<GameActor>()->GetEventSubject().AddObserver(this);
    ActorType type = entity->GetComponent<GameActor>()->GetActorType();

    if (type == ActorType::player)
        m_Players += 1;
    if (type == ActorType::enemy)
        m_enemies += 1;

    std::cout << "players: " << m_Players << '\n';
    std::cout << "enemies: " << m_enemies << '\n';
}

void Tron::GameManager::CheckWinCondition()
{
    std::cout << "Checking Win Condition\n";

        switch (m_CurrentMode)
        {
        case GameMode::singlePlayer:
            if (m_Players == 0)
                LevelManager::GetInstance().LoadLevel("", LevelCategory::Menu);
            if (m_enemies == 0)
            {
                std::cout << "loading Menu\n";
                LevelManager::GetInstance().LoadLevel("", LevelCategory::Menu);
            }
        case GameMode::COOP:
            if (m_Players == 0)
                LevelManager::GetInstance().LoadLevel("", LevelCategory::Menu);
            if (m_enemies == 0)
                LevelManager::GetInstance().LoadLevel("", LevelCategory::Menu);
            break;
        case GameMode::PVP:
            if (m_Players <= 1)
                LevelManager::GetInstance().LoadLevel("", LevelCategory::Menu);
            break;
        }
    
}



