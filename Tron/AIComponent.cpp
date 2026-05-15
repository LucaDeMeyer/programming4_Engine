#include "AIComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Command.h"
#include "LevelManager.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include "GameTime.h"
#include "TankCommands.h"
#include "States.h"

Tron::AIComponent::AIComponent(dae::GameObject* owner)
    : BaseComponent(owner)
    , m_CurrentState(std::make_unique<PatrolState>())
{
}

void Tron::AIComponent::Update()
{
    if (!m_pMoveCommand || !m_CurrentState) return;

    if (EnemyState* next = m_CurrentState->Update(*this))
        TransitionTo(next);

    m_pMoveCommand->SetDirection(m_CurrentDirection * 100.f);
    m_pMoveCommand->Execute();

    m_LastFireTime += Time::GetInstance().GetDeltaTime(); 

    if (m_LastFireTime >= m_FireCooldown)
    {
        if (CanSeePlayer())
        {
            m_pFireCommand->Execute();
            m_LastFireTime = 0.0f; 
        }
    }

}

void Tron::AIComponent::TransitionTo(EnemyState* newState)
{
    m_CurrentState->OnExit(*this);
    m_CurrentState.reset(newState);
    m_CurrentState->OnEnter(*this);
}

bool Tron::AIComponent::IsAtTileCenter() const
{
    const auto pos = GetOwner()->GetTransform()->GetLocalPosition();

    float offsetX = LevelManager::GetInstance().GetOffsetX();
    float offsetY = LevelManager::GetInstance().GetOffsetY();

    float gridX = pos.x - offsetX;
    float gridY = pos.y - offsetY;

    float modX = fmod(gridX, m_TileSize);
    float modY = fmod(gridY, m_TileSize);
    const float threshold = 2.0f;

    return (modX < threshold || modX > m_TileSize - threshold) &&
        (modY < threshold || modY > m_TileSize - threshold);
}

void Tron::AIComponent::ChooseNewDirection()
{
    std::vector<glm::vec3> validDirs;
    glm::vec3 potentials[] = { {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0} };

    for (const auto& dir : potentials)
    {
        if (dir == -m_CurrentDirection) continue;
        if (!IsPathBlocked(dir))
            validDirs.push_back(dir);
    }

    if (!validDirs.empty())
        m_CurrentDirection = validDirs[rand() % validDirs.size()];
    else
        m_CurrentDirection = -m_CurrentDirection;
}

void Tron::AIComponent::ChasePlayer()
{
    const auto myPos = GetOwner()->GetTransform()->GetLocalPosition();
    auto* player = LevelManager::GetInstance().GetNearestPlayer(myPos);
    if (!player)
    {
        ChooseNewDirection();
        return;
    }

    const auto playerPos = player->GetTransform()->GetLocalPosition();
    glm::vec3 diff = playerPos - myPos;
    glm::vec3 toPlayer = glm::vec3(diff.x, diff.y, 0);

    std::vector<std::pair<float, glm::vec3>> scored;
    glm::vec3 potentials[] = { {1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0} };

    for (const auto& dir : potentials)
    {
        if (IsPathBlocked(dir)) continue;
        float score = glm::dot(toPlayer, dir);
        scored.emplace_back(score, dir);
    }

    if (!scored.empty())
    {
        auto best = std::max_element(scored.begin(), scored.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        m_CurrentDirection = best->second;
    }
}

bool Tron::AIComponent::IsPathBlocked(const glm::vec3& dir) const
{
    const auto pos = GetOwner()->GetTransform()->GetLocalPosition();
    glm::vec3 checkPos = pos + (dir * m_TileSize);
    return Tron::LevelManager::GetInstance().IsWallAt(checkPos);
}

bool Tron::AIComponent::CanSeePlayer() const
{
    const auto myPos = GetOwner()->GetTransform()->GetLocalPosition();
    auto* player = LevelManager::GetInstance().GetNearestPlayer(myPos);
    if (!player)
    {

        return false;
    }

    const auto playerPos = player->GetTransform()->GetLocalPosition();

    float diffY = std::abs(myPos.y - playerPos.y);
    float diffX = std::abs(myPos.x - playerPos.x);


    bool sameRow = diffY < m_TileSize * 0.5f;
    bool sameCol = diffX < m_TileSize * 0.5f;

  

    if (!sameRow && !sameCol) return false;

    glm::vec3 dir{};
    if (sameRow)
        dir = { playerPos.x > myPos.x ? 1.f : -1.f, 0, 0 };
    else
        dir = { 0, playerPos.y > myPos.y ? 1.f : -1.f, 0 };

    float dist = sameRow ? diffX : diffY;
    if (dist > m_TileSize * 6.f) return false;

    glm::vec3 check = myPos + dir * m_TileSize;
    float traveled = m_TileSize;
    while (traveled < dist)
    {
        bool blocked = LevelManager::GetInstance().IsWallAt(check);
        if (blocked) return false;
        check += dir * m_TileSize;
        traveled += m_TileSize;
    }

    return true;
}

void Tron::AIComponent::SnapToGrid()
{
    auto* transform = GetOwner()->GetTransform();
    auto pos = transform->GetLocalPosition();

    float offX = Tron::LevelManager::GetInstance().GetOffsetX();
    float offY = Tron::LevelManager::GetInstance().GetOffsetY();

    float snappedX = std::round((pos.x - offX) / m_TileSize) * m_TileSize + offX;
    float snappedY = std::round((pos.y - offY) / m_TileSize) * m_TileSize + offY;

    transform->SetLocalPosition({ snappedX, snappedY, pos.z });
}