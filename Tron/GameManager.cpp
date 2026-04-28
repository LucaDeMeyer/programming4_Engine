#include "GameManager.h"
#include <iostream>
#include "EventQueue.h"
#include "GameActorComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "TronEvents.h"
#include "TransformComponent.h"

//should handle score saving/lives saving between lvls in here or again reuse GOs instead of making new ones each lvl swap
void Tron::GameManager::Init()
{
    dae::EventQueue::GetInstance().GetNotifier()->AddObserver(&Tron::GameManager::GetInstance());
}

void Tron::GameManager::OnNotify(dae::GameObject* pEntity, const dae::Event& event)
{
    if (event.ID == dae::Utils::make_sdbm_hash("ActorDied"))
    {
        auto* data = static_cast<ActorDied*>(event.pArgs.get());

        if (data && data->obj)
        {
            RemoveEntity(data->obj);

            dae::Event winEvent(dae::Utils::make_sdbm_hash("WinCondition"));
            dae::EventQueue::GetInstance().AddEvent(std::move(winEvent));

        }
    }

    if (event.ID == dae::Utils::make_sdbm_hash("WinCondition"))
    {
        CheckWinCondition();
    }

    if (event.ID == dae::Utils::make_sdbm_hash("Teleport"))
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
}

void Tron::GameManager::CheckWinCondition()
{
   switch (m_CurrentMode)
    {
    case GameMode::singlePlayer:
        if (m_Players == 0)
            LevelManager::GetInstance().RequestLevel("", LevelCategory::Menu);
        if (m_enemies == 0)
        {
            ++m_LVLNR;
            if(m_LVLNR <=3)
                LevelManager::GetInstance().RequestLevel("Data/Level" + std::to_string(m_LVLNR) + ".csv", LevelCategory::Game);
            else
                LevelManager::GetInstance().RequestLevel("", LevelCategory::Menu);
        }
        break;
    case GameMode::COOP:
        if (m_Players == 0)
            LevelManager::GetInstance().RequestLevel("", LevelCategory::Menu);
        if (m_enemies == 0)
        {
            ++m_LVLNR;
            if (m_LVLNR <= 3)
                LevelManager::GetInstance().RequestLevel("Data/Level" + std::to_string(m_LVLNR) + ".csv", LevelCategory::Game);
            else
                LevelManager::GetInstance().RequestLevel("", LevelCategory::Menu);
        }
        break;
    case GameMode::PVP:
        if (m_Players <= 1)
            LevelManager::GetInstance().RequestLevel("", LevelCategory::Menu);
        break;
    }
  
}



