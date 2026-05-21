#include "GameManager.h"
#include <iostream>
#include "EventQueue.h"
#include "GameActorComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "TronEvents.h"
#include "TransformComponent.h"
#include <filesystem>
#include <fstream>

void Tron::GameManager::Init()
{
    dae::EventQueue::GetInstance().GetNotifier()->AddObserver(&Tron::GameManager::GetInstance());
    LoadFile();
}

void Tron::GameManager::OnNotify(dae::GameObject* pEntity, const dae::Event& event)
{
    if (m_IsTransitioningLevel) return;

    if (event.ID == dae::Utils::make_sdbm_hash("ActorDied"))
    {
        if (!event.pArgs) return;

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
    if (event.ID == dae::Utils::make_sdbm_hash("ScoreChangedEvent"))
    {
        if (!event.pArgs) return;
        auto* data = static_cast<ScoreGainedARGS*>(event.pArgs.get());
        if (data->playerIndex == 0)
            m_P1Score = data->points;
        else
            m_p2Score = data->points;
    }

    if (event.ID == dae::Utils::make_sdbm_hash("Teleport"))
    {
        if (!event.pArgs) return;
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
    if (!entity || entity->IsMarkedForDestruction()) return;

    auto gameActor = entity->GetComponent<GameActor>();
    if (!gameActor) return;

    ActorType type = gameActor->GetActorType();

    if (type == ActorType::player) m_Players -= 1;
    if (type == ActorType::enemy)  m_enemies -= 1;

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
    if (m_IsTransitioningLevel || m_Entities.empty()) return;

    switch (m_CurrentMode)
    {
    case GameMode::singlePlayer:
        if (m_Players <= 0)
        {
            m_IsTransitioningLevel = true; 
            LevelManager::GetInstance().GoToMenu();
            AddScore("Player 1", m_P1Score);

            m_P1Score = 0;
            m_LVLNR = 0;
        }
        else if (m_enemies <= 0)
        {
            m_IsTransitioningLevel = true; 
            ++m_LVLNR;
            LevelManager::GetInstance().NextLevel();
        }
        break;

    case GameMode::COOP:
        if (m_Players <= 0)
        {
            m_IsTransitioningLevel = true; 
            LevelManager::GetInstance().GoToMenu();
            AddScore("Player 1", m_P1Score);
            AddScore("Player 2", m_p2Score);

            m_P1Score = 0;
            m_p2Score = 0;
            m_LVLNR = 0;
        }
        else if (m_enemies <= 0)
        {
            m_IsTransitioningLevel = true;
            ++m_LVLNR;
            LevelManager::GetInstance().NextLevel();
        }
        break;

    case GameMode::PVP:
        if (m_Players <= 1)
        {
            m_IsTransitioningLevel = true;
            LevelManager::GetInstance().GoToMenu();
        }
        break;
    }
}

void Tron::GameManager::AddScore(const std::string& name, int score)
{
    std::filesystem::path savePath{ m_FileName };
    if (savePath.has_parent_path() && !std::filesystem::exists(savePath.parent_path()))
    {
        std::filesystem::create_directories(savePath.parent_path());
    }

    m_HighScores.push_back({ name, score });

    std::sort(m_HighScores.begin(), m_HighScores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b) {
        return a.score > b.score;
        });

    if (m_HighScores.size() > 10) m_HighScores.resize(10);
    SaveToFile();
}

void Tron::GameManager::SaveToFile()
{
    std::ofstream file(m_FileName);
    for (const auto& entry : m_HighScores) {
        file << entry.name << "," << entry.score << "\n";
    }
}

void Tron::GameManager::LoadFile()
{
    if (!std::filesystem::exists(m_FileName))
    {
        std::cout << "No highscore file found. Starting fresh.\n";
        return;
    }

    std::ifstream file(m_FileName);
    std::string line;
    m_HighScores.clear();

    while (std::getline(file, line)) {
        if (line.empty()) continue; 

        size_t commaPos = line.find(',');
        if (commaPos != std::string::npos) {
            try {
                std::string name = line.substr(0, commaPos);
                int score = std::stoi(line.substr(commaPos + 1));
                m_HighScores.push_back({ name, score });
            }
            catch (...) {
                // skip corrupted lines
                continue;
            }
        }
    }
}


