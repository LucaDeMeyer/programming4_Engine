#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <filesystem>
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Minigin.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "TronFactory.h"
#include "BulletManager.h"
#include "ParticleManager.h"
#include "ServiceLocator.h"
#include "TronServices.h"


/*
 * TODO: 
 * clean up project -> move files to seperate folders X
 * particle manager X
 * clean up Cmake
 * maybe profile cache misses and hits => tracy profiler?
 * add player aiming with arrow keys x
 * winner screen during PVP battles after each round / current score X
 * FIX BULLET-WALL Collisions
 */
namespace fs = std::filesystem;

namespace Tron::Config // this might not be the best solution
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
	auto& ParticleManager = Tron::ParticleManager::GetInstance();
	ParticleManager.Init();

	dae::ServiceLocator::RegisterGameService(std::make_unique<Tron::TronGameLoopService>());

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
