#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "ActorCommands.h"
#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "LivesComponent.h"
#include "LivesDisplay.h"
#include "MovementComponent.h"
#include "RotationComponent.h"
#include "ScoreDisplay.h"
#include "TankCommands.h"
#include "Tank_Bullet.h"
#include "Tank_CollisionObserver.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TileMapComponent.h"
#include "TransformComponent.h"

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include <filesystem>
#include "TronFactory.h"
#include "AchievmentManager.h"
#include "BulletManager.h"
#include "ServiceLocator.h"
#include "TronServices.h"


/*
 * TODO: 
 * clean up project -> move files to seperate folders
 * particle manager
 * clean up Cmake
 * maybe profile cache misses and hits
 * add player aiming with arrow keys
 * winner screen during PVP battles after each round / current score X
 */
namespace fs = std::filesystem;

namespace Tron::Config
{
	inline constexpr int WINDOW_WIDTH = 1240;
	inline constexpr int WINDOW_HEIGHT = 800;

	inline glm::vec2 GetWindowSize() {
		return { static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) };
	}
}

static void load()
{
	auto& GameManager = Tron::GameManager::GetInstance();
	GameManager.SetWindowSize(Tron::Config::GetWindowSize());
	Tron::EnemyDatabase::GetInstance().Init();
	auto& levelManager = Tron::LevelManager::GetInstance();
	GameManager.Init();
	levelManager.Init();
	auto& BulletManager = Tron::BulletManager::GetInstance();
	BulletManager.Init();

	dae::ServiceLocator::RegisterPlatform(std::make_unique<Tron::LevelManagerService>());

	dae::ServiceLocator::RegisterWeaponService(std::make_unique<Tron::BulletPoolService>());
}

int main(int, char* []) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "Data/";
	if (!fs::exists(data_location))
		data_location = "./Data/";
#endif
#if USE_STEAMWORKS
	dae::ServiceLocator::RegisterPlatform(
		std::make_unique<Tron::SteamPlatformService>()
	);
	dae::ServiceLocator::RegisterAchievements(
		std::make_unique<Tron::SteamAchievementService>()
	);
#endif
	dae::Minigin engine(data_location,Tron::Config::GetWindowSize());
	engine.Run(load);

	return 0;
}
