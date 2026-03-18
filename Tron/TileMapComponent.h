#ifndef TILE_MAP_COMPONENT_H
#define TILE_MAP_COMPONENT_H
#include <memory>
#include <string>
#include <vector>

#include "BaseComponent.h"
#include "Texture2D.h"

enum class TileType : int
{
    Black = 0,
    Wall = 1,
    VerticalPath = 2,
    HorizontalPath = 3,
    Crossroad = 4
};

class TileMapComponent : public dae::BaseComponent
{
public:
    TileMapComponent(dae::GameObject* owner, float tileSize);

    TileMapComponent(const TileMapComponent& other) = delete;
    TileMapComponent(TileMapComponent&& other) = delete;
    TileMapComponent& operator=(TileMapComponent&& other) = delete;
    TileMapComponent& operator=(const TileMapComponent& other) = delete;

    void LoadLevel(const std::string& csvFilePath);
    void Render() const override;
    void Update() override{}
    void RenderUI() override{};
private:
    int m_Cols;
    int m_Rows;
    float m_TileSize;
    std::vector<TileType> m_Grid;

    std::shared_ptr<dae::Texture2D> m_WallTex;
    std::shared_ptr<dae::Texture2D> m_PathVTex;
    std::shared_ptr<dae::Texture2D> m_PathHTex;
    std::shared_ptr<dae::Texture2D> m_PathCTex;
    std::shared_ptr<dae::Texture2D> m_BlackTex;

};
#endif
