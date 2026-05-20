#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <string>
#include <vector>
#include "Singleton.h"
#include <glm/vec3.hpp>

#include "GameObject.h"
#include "Services.h"

namespace dae
{
	class Scene;
}

namespace Tron
{
	enum class Team;

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
	class LevelManager :public dae::Singleton<LevelManager>
	{
	public:
		void Init();
		void Update() ;

		void LoadLevel(LevelCategory category);

		void NextLevel();

		TileType GetTileAt(float worldX, float worldY) const;
		bool IsWallAt(const glm::vec3& worldPos) const;

		glm::vec3 GetRandomPathLocation();

		float GetOffsetX() const { return m_OffsetX; }

		float GetOffsetY() const { return m_OffsetY; }

		void RequestLevel(LevelCategory category);

		dae::GameObject* GetNearestPlayer(const glm::vec3& pos) const;
	private:
		friend class dae::Singleton<LevelManager>;
		LevelManager() = default;

		void LoadGrid( std::string& path,dae::Scene& scene);
		void LoadMenu(dae::Scene& scene);
		std::string GetTextureForType(TileType type);

		void ParseGrid( std::string& path, dae::Scene& scene);
		void SpawnPlayers( dae::Scene& scene);
		void SpawnSinglePlayer( dae::Scene& scene, int playerIndex, const glm::vec3& spawnPos, const std::string& texture, Tron::Team team);
		void SpawnEnemies(dae::Scene& scene);

		void CreateMenuButton(dae::Scene& scene, const std::string& text, const glm::vec3& pos, std::function<void()> callback);
		void CreateFPSCounter(dae::Scene& scene, const glm::vec3& pos);
		void SetupLevelAudio();


		
		int m_Cols;
		int m_Rows;
		float m_TileSize{32.f};
		float m_OffsetX;
		float m_OffsetY;
		glm::vec3 m_P1Spawn;
		glm::vec3 m_P2Spawn;
		std::vector<glm::vec3> m_EnemySpawnPoints;
		glm::vec3 m_CenterTile;
		std::vector<glm::vec3> m_EmptyLocations;
		std::vector<TileType> m_Grid;

		bool m_PendingLoad = false;
		std::string m_PendingPath;
		LevelCategory m_PendingCategory = LevelCategory::Menu;

		std::vector<std::string> m_LevelFiles{ "Data/Level1.csv", "Data/Level2.csv", "Data/Level3.csv" };
		int m_LevelPlaylistIndex = 0;


		size_t m_CurrentLevelIndex{ 0 };

		dae::GameObject* m_Pplayer1{};
		dae::GameObject* m_Pplayer2{};
	};
}

#endif
