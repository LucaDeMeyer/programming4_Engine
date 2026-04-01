#include "LevelManager.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "TransformComponent.h"

#include <fstream>
#include <sstream>

#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "SpriteComponent.h"
#include "TextureComponent.h"

void Tron::LevelManager::LoadLevel(const std::string& path,LevelCategory category)
{
    if (category == LevelCategory::Menu) {
        LoadMenu(path); 
    }
    else {
        LoadGrid(path); 
    }
}

void Tron::LevelManager::LoadGrid(const std::string& path)
{
    m_Grid.clear();
    m_Rows = 0;
    m_Cols = 0;

    std::ifstream file(path);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int currentLineCols = 0;

        while (std::getline(ss, cell, ',')) {
            m_Grid.push_back(static_cast<TileType>(std::stoi(cell)));
            currentLineCols++;
        }
        if (m_Cols == 0) m_Cols = currentLineCols;
        m_Rows++;
    }

    auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

    float totalLevelWidth = m_Cols * m_TileSize;
    float totalLevelHeight = m_Rows * m_TileSize;

    float windowWidth = 1024.f;
    float windowHeight = 576.f;

    float offsetX = (windowWidth - totalLevelWidth) / 2.0f;
    float offsetY = (windowHeight - totalLevelHeight) / 2.0f;

    for (size_t i = 0; i < m_Grid.size(); ++i) {
        float x = ((i % m_Cols) * m_TileSize) + offsetX;
        float y = ((i / m_Cols) * m_TileSize) + offsetY;

        auto tile = std::make_unique<dae::GameObject>();
        tile->GetTransform()->SetLocalPosition(glm::vec3{ x, y ,0 });

        std::string texName = GetTextureForType(m_Grid[i]);
        tile->AddComponent<dae::TextureComponent>()->SetTexture(texName);
        tile->GetComponent<dae::TextureComponent>()->SetDimensions(m_TileSize, m_TileSize);
        if (m_Grid[i] == TileType::Wall) {
            tile->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, m_TileSize, m_TileSize });
            tile->AddComponent<FactionComponent>(Team::Wall);
        }

        scene.Add(std::move(tile));
    }
}

void Tron::LevelManager::LoadMenu(const std::string& path)
{
    auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

    auto background = std::make_unique<dae::GameObject>();
    background->AddComponent<dae::TextureComponent>()->SetTexture("place_holder.png");
    scene.Add(std::move(background));

    //some sort of basic UI comp?
    // buttons how will we handle these?
    //text
    //could use IMGUI
}


std::string Tron::LevelManager::GetTextureForType(TileType type) {
    switch (type) {
    case TileType::Wall:           return "Tile_Wall.png";
    case TileType::VerticalPath:   return "Tile_vertical.png";
    case TileType::HorizontalPath: return "Tile_Horizontal.png";
    case TileType::Crossroad:      return "Tile_Crossing.png";
    case TileType::Black:          return "Tile_Black.png";
    default: throw std::runtime_error("Unknown tile type, failed to load level");
    }
}
