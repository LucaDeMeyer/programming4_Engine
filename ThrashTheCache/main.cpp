#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "FPSComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "ThrashTheCacheUI.h"
#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>()->SetTexture("logo.png");
	go->GetTransform()->SetLocalPosition({ 358, 180,1 });
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::TextComponent>()
		->SetText("Programming 4 Assignment")
		->SetFont("Lingua.otf", 36)
		->SetColor(255, 255, 255, 255);
	to->GetTransform()->SetLocalPosition({ 292, 20,1 });
	scene.Add(std::move(to));

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>()
		->SetText("FPS")
		->SetText("FPS")
		->SetFont("lingua.otf", 15)
		->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->GetTransform()->SetLocalPosition({ 50, 20,1 });
	scene.Add(std::move(fps));

	auto CacheUI = std::make_unique<dae::GameObject>();
	CacheUI->AddComponent<dae::ThrashTheCacheUI>();

	scene.Add(std::move(CacheUI));
}

int main(int, char* []) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
	return 0;
}
