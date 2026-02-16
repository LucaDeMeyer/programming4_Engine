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

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>();
	go->GetComponent<dae::TextureComponent>()->SetTexture("background.png");
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>();
	go->GetComponent<dae::TextureComponent>()->SetTexture("logo.png");
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::TextComponent>();
	to->GetComponent<dae::TextComponent>()->SetText("Programming 4 Assignment");
	to->GetComponent<dae::TextComponent>()->SetFont("lingua.otf", 36);
	to->GetComponent<dae::TextComponent>()->SetColor( 255, 255, 0, 255 );
	to->AddComponent<dae::TransformComponent>();
	to->GetComponent<dae::TransformComponent>()->SetPosition(292, 20);
	scene.Add(std::move(to));

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>();
	fps->GetComponent<dae::TextComponent>()->SetText("FPS");
	fps->GetComponent<dae::TextComponent>()->SetFont("lingua.otf", 15);
	fps->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->AddComponent<dae::TransformComponent>();
	fps->GetComponent<dae::TransformComponent>()->SetPosition(50, 20);
	scene.Add(std::move(fps));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
