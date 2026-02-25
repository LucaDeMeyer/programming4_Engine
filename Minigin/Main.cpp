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

#include "RotationComponent.h"
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
	go->GetTransform()->SetLocalPosition({ 358, 180,1 });
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto to = std::make_unique<dae::GameObject>();
	to->AddComponent<dae::TextComponent>();
	to->GetComponent<dae::TextComponent>()->SetText("Programming 4 Assignment");
	to->GetComponent<dae::TextComponent>()->SetFont("lingua.otf", 36);
	to->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	to->GetTransform()->SetLocalPosition({ 292, 20,1 });
	scene.Add(std::move(to));

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>();
	fps->GetComponent<dae::TextComponent>()->SetText("FPS");
	fps->GetComponent<dae::TextComponent>()->SetFont("lingua.otf", 15);
	fps->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->GetTransform()->SetLocalPosition({ 50, 20,1 });
	scene.Add(std::move(fps));

	auto pivot = std::make_unique<dae::GameObject>();
	pivot->GetTransform()->SetLocalPosition({ 300, 300, 1});
	pivot->AddComponent<dae::RotationComponent>();
	pivot->GetComponent<dae::RotationComponent>()->SetRotationSpeed(-180.f);

	auto tank_1 = std::make_unique<dae::GameObject>();
	tank_1->GetTransform()->SetLocalPosition({ 60, 10,1 });
	tank_1->AddComponent<dae::TextureComponent>();
	tank_1->GetComponent<dae::TextureComponent>()->SetTexture("Red_Tank.png");
	tank_1->AddComponent<dae::RotationComponent>();
	tank_1->GetComponent<dae::RotationComponent>()->SetRotationSpeed(180.f);


	auto tank_2 = std::make_unique<dae::GameObject>();
	tank_2->GetTransform()->SetLocalPosition({ 60, 0,1 });
	tank_2->AddComponent<dae::TextureComponent>();
	tank_2->GetComponent<dae::TextureComponent>()->SetTexture("Blue_Tank.png");

	tank_1->SetParent(pivot.get(), false);
	tank_2->SetParent(tank_1.get(), false);

	scene.Add(std::move(pivot));
	scene.Add(std::move(tank_1));
	scene.Add(std::move(tank_2));
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
