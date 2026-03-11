#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "ActorCommands.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "MovementComponent.h"
#include "RotationComponent.h"
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
		->SetFont("lingua.otf", 36)
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


	auto tank_1 = std::make_unique<dae::GameObject>();
	tank_1->GetTransform()->SetLocalPosition({ 60, 100,1 });
	tank_1->AddComponent<dae::TextureComponent>()->SetTexture("Red_Tank.png");
	tank_1->AddComponent<dae::MovementComponent>();
	tank_1->GetComponent<dae::MovementComponent>()->SetMovementSpeed(50.0f);

	auto moveUpCommand = std::make_unique<dae::MoveVertical>(tank_1.get(),dae::Direction::Negative);
	auto MoveLeftCommand = std::make_unique<dae::MoveHorizontal>(tank_1.get(),dae::Direction::Negative);
	auto moveDownCommand = std::make_unique<dae::MoveVertical>(tank_1.get(), dae::Direction::Positive);
	auto MoveRightCommand = std::make_unique<dae::MoveHorizontal>(tank_1.get(), dae::Direction::Positive);

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_W,
		dae::InputState::Pressed,
		std::move(moveUpCommand)
	);

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_S,
		dae::InputState::Pressed,
		std::move(moveDownCommand)
	);

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_A,
		dae::InputState::Pressed,
		std::move(MoveLeftCommand)
	);

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_D,
		dae::InputState::Pressed,
		std::move(MoveRightCommand)
	);



	auto tank_2 = std::make_unique<dae::GameObject>();
	tank_2->GetTransform()->SetLocalPosition({ 60, 200,1 });
	tank_2->AddComponent<dae::TextureComponent>()->SetTexture("Blue_Tank.png");
	tank_2->AddComponent<dae::MovementComponent>();
	tank_2->GetComponent<dae::MovementComponent>()->SetMovementSpeed(100.0f);


	auto moveUpCommand2 = std::make_unique<dae::MoveVertical>(tank_2.get(), dae::Direction::Negative);
	auto MoveLeftCommand2 = std::make_unique<dae::MoveHorizontal>(tank_2.get(), dae::Direction::Negative);
	auto moveDownCommand2 = std::make_unique<dae::MoveVertical>(tank_2.get(), dae::Direction::Positive);
	auto MoveRightCommand2 = std::make_unique<dae::MoveHorizontal>(tank_2.get(), dae::Direction::Positive);

	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::DPadLeft,
		dae::InputState::Pressed, std::move(MoveLeftCommand2));
	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::DPadRight,
		dae::InputState::Pressed, std::move(MoveRightCommand2));
	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::DPadUp,
		dae::InputState::Pressed, std::move(moveUpCommand2));
	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::DPadDown,
		dae::InputState::Pressed, std::move(moveDownCommand2));


	scene.Add(std::move(tank_1));
	scene.Add(std::move(tank_2));
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
