#include "AIComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Command.h"
#include "LevelManager.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "TankCommands.h"

void Tron::AIComponent::Update()
{
    if (!m_pMoveCommand) return;

    bool atCenter = IsAtTileCenter();

    if (atCenter && !m_MadeDecisionThisTile)
    {
        SnapToGrid();

        if (IsPathBlocked(m_CurrentDirection) || ShouldChangeDirection())
        {
            ChooseNewDirection();
        }

        m_MadeDecisionThisTile = true; 
    }
    else if (!atCenter)
    {
        m_MadeDecisionThisTile = false;
    }
  
    m_pMoveCommand->SetDirection(m_CurrentDirection * 100.0f);
    m_pMoveCommand->Execute();
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
        {
            validDirs.push_back(dir);
        }
    }

    if (!validDirs.empty())
    {
        m_CurrentDirection = validDirs[rand() % validDirs.size()];
    }
    else
    {
        m_CurrentDirection = -m_CurrentDirection;
    }
}

bool Tron::AIComponent::IsPathBlocked(const glm::vec3& dir) const
{
    const auto pos = GetOwner()->GetTransform()->GetLocalPosition();

    glm::vec3 checkPos = pos + (dir * (m_TileSize ));

    return Tron::LevelManager::GetInstance().IsWallAt(checkPos);
}

bool Tron::AIComponent::ShouldChangeDirection() const
{
    return (rand() % 100) < 20;
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