#include "TileMapComponent.h"

#include <iostream>

#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "fstream"
TileMapComponent::TileMapComponent(dae::GameObject* owner, float tileSize)
    : BaseComponent(owner),m_Rows(),m_Cols(), m_TileSize(tileSize)
{
    auto& rm = dae::ResourceManager::GetInstance();
    m_WallTex = rm.LoadTexture("Tile_Wall.png");
    m_PathVTex = rm.LoadTexture("Tile_vertical.png");
    m_PathHTex = rm.LoadTexture("Tile_Horizontal.png");
    m_PathCTex = rm.LoadTexture("Tile_Crossing.png");
    m_BlackTex = rm.LoadTexture("Tile_Black.png");
}

void TileMapComponent::Render() const
{
    auto pos = GetOwner()->GetTransform()->GetWorldPosition();

    for (size_t i = 0; i < m_Grid.size(); ++i)
    {
        int gridX = i % m_Cols;
        int gridY = i / m_Cols;

        float screenX = pos.x + (gridX * m_TileSize);
        float screenY = pos.y + (gridY * m_TileSize);

        switch (m_Grid[i])
        {
        case TileType::Wall:
            dae::Renderer::GetInstance().RenderTexture(*m_WallTex, screenX, screenY, m_TileSize, m_TileSize);
            break;
        case TileType::VerticalPath:
            dae::Renderer::GetInstance().RenderTexture(*m_PathVTex, screenX, screenY, m_TileSize, m_TileSize);
            break;
        case TileType::Crossroad:
            dae::Renderer::GetInstance().RenderTexture(*m_PathCTex, screenX, screenY, m_TileSize, m_TileSize);
            break;
        case TileType::HorizontalPath:
            dae::Renderer::GetInstance().RenderTexture(*m_PathHTex, screenX, screenY, m_TileSize, m_TileSize);
            break;
        case TileType::Black:
            dae::Renderer::GetInstance().RenderTexture(*m_BlackTex, screenX, screenY, m_TileSize, m_TileSize);
            break;
        }
    }
}
void TileMapComponent::LoadLevel(const std::string& csvFilePath)
{
    m_Grid.clear();
    m_Cols = 0;
    m_Rows = 0;

    std::ifstream file(csvFilePath);
    if (!file.is_open())
    {
        std::cerr << "TileMapComponent: Failed to open CSV file: " << csvFilePath << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        int currentCols = 0;

        while (std::getline(ss, cell, ','))
        {
            int tileID = std::stoi(cell);
            m_Grid.push_back(static_cast<TileType>(tileID));

            currentCols++;
        }
        if (m_Cols == 0) m_Cols = currentCols;

        m_Rows++;
    }
}
