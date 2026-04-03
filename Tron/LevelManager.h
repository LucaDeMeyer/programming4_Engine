#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <string>
#include <vector>
#include "Singleton.h"
#include <glm/vec3.hpp>
namespace dae
{
	class Scene;
}

namespace Tron
{

	enum class TileType : int
	{
		Black = 0,
		Wall = 1,
		VerticalPath = 2,
		HorizontalPath = 3,
		Crossroad = 4,
		P1Spawn = 5,
		P2Spawn = 6,
		EnemySpawn = 7,
		CenterTile = 8
	};


	enum class LevelCategory { Menu, Game };
	class LevelManager : public dae::Singleton<LevelManager>
	{
	public:
		void Init();
		void LoadLevel(const std::string& path,LevelCategory category);

		void NextLevel();

		TileType GetTileAt(float worldX, float worldY) const;

		glm::vec3 GetRandomPathLocation();
	private:
		friend class dae::Singleton<LevelManager>;
		LevelManager() = default;

		void LoadGrid(const std::string& path,dae::Scene& scene);
		void LoadMenu(dae::Scene& scene);
		std::string GetTextureForType(TileType type);
		int m_Cols;
		int m_Rows;
		float m_TileSize{32.f};
		glm::vec3 m_P1Spawn;
		glm::vec3 m_P2Spawn;
		std::vector<glm::vec3> m_EnemySpawnPoints;
		glm::vec3 m_CenterTile;
		std::vector<glm::vec3> m_EmptyLocations;
		std::vector<TileType> m_Grid;

	};
}

#endif
