#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "ActorCommands.h"
#include "ColliderComponents.h"
#include "FactionComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
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

namespace fs = std::filesystem;

static void load()
{
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	dae::SceneManager::GetInstance().SetActiveScene(0);
	Tron::AchievementManager::GetInstance().Init();

	auto player = Tron::GOFactory::CreatePlayer({ 60, 250, 1 });

	auto LivesDisplayTank_1 = std::make_unique<dae::GameObject>();
	LivesDisplayTank_1->AddComponent<Tron::LivesDisplay>(player.Base->GetComponent<Tron::LivesComponent>()->GetLives());
	LivesDisplayTank_1->GetComponent<Tron::LivesDisplay>()->SetTexture("Player_Lives.png"); 
	LivesDisplayTank_1->GetTransform()->SetLocalPosition({ 60, 10, 1 });
	player.Base->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(LivesDisplayTank_1->GetComponent<Tron::LivesDisplay>());

	auto ScoreDisplay1 = std::make_unique<dae::GameObject>();
	ScoreDisplay1->GetTransform()->SetLocalPosition({ 200, 10, 0 });
	ScoreDisplay1->AddComponent<dae::TextComponent>();
	ScoreDisplay1->AddComponent<Tron::ScoreDisplay>();
	player.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(ScoreDisplay1->GetComponent<Tron::ScoreDisplay>());

	player.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(&Tron::AchievementManager::GetInstance());

	auto moveUpCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 0,-100 });
	auto MoveLeftCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ -100,0 });
	auto moveDownCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 0,100 });
	auto MoveRightCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 100,0 });
	auto fireCommand = std::make_unique<Tron::FireCommand>(player.Base.get()); 

	auto damageCommand = std::make_unique<Tron::DamageCommand>(player.Base.get(),2);

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

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_SPACE,
		dae::InputState::Down,
		std::move(fireCommand));

	dae::InputManager::GetInstance().BindKeyCommand(
		SDLK_C,
		dae::InputState::Down,
		std::move(damageCommand));

	auto tank_2 = Tron::GOFactory::CreatePlayer({ 60,200,1 });

	auto LivesDisplayTank_2 = std::make_unique<dae::GameObject>();
	LivesDisplayTank_2->AddComponent<Tron::LivesDisplay>(tank_2.Base->GetComponent<Tron::LivesComponent>()->GetLives());
	LivesDisplayTank_2->GetComponent<Tron::LivesDisplay>()->SetTexture("Player_Lives.png");
	LivesDisplayTank_2->GetTransform()->SetLocalPosition({600, 10, 1 });
	tank_2.Base->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(LivesDisplayTank_2->GetComponent<Tron::LivesDisplay>());

	auto ScoreDisplay2 = std::make_unique<dae::GameObject>();
	ScoreDisplay2->GetTransform()->SetLocalPosition({ 400, 10, 0 });
	ScoreDisplay2->AddComponent<dae::TextComponent>();
	ScoreDisplay2->AddComponent<Tron::ScoreDisplay>();
	tank_2.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(ScoreDisplay2->GetComponent<Tron::ScoreDisplay>());

	tank_2.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(&Tron::AchievementManager::GetInstance());

	auto moveUpCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(),glm::vec2{0,-100});
	auto MoveLeftCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{-100,0});
	auto moveDownCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{ 0,100 });
	auto MoveRightCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{100,0});
	auto fireCommand2 = std::make_unique<Tron::FireCommand>(tank_2.Base.get());

	auto DamageTest = std::make_unique<Tron::DamageCommand>(tank_2.Base.get(), 1);

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
	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::ButtonA,
		dae::InputState::Down,std::move(fireCommand2));
	dae::InputManager::GetInstance().BindControllerCommand(
		0, dae::Controller::ControllerButton::ButtonB,
		dae::InputState::Down, std::move(DamageTest));

	auto enemyTank_01 = Tron::GOFactory::CreateEnemy({ 150,300,1 });
	auto enemyTank_02 = Tron::GOFactory::CreateEnemy({ 200,300,1 });
	auto enemyTank_03 = Tron::GOFactory::CreateEnemy({ 250,300,1 });
	auto enemyTank_04 = Tron::GOFactory::CreateEnemy({ 100,300,1 });
	auto enemyTank_05 = Tron::GOFactory::CreateEnemy({ 300,500,1 });
	auto enemyTank_06 = Tron::GOFactory::CreateEnemy({ 250,530,1 });

	auto controlDisplay1 = std::make_unique<dae::GameObject>();
	controlDisplay1->GetTransform()->SetLocalPosition({ 60,100,1 });
	controlDisplay1->AddComponent<dae::TextComponent>();
	controlDisplay1->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 10);
	controlDisplay1->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	controlDisplay1->GetComponent<dae::TextComponent>()->SetText("Use WASD to move Red tank, space to shoot, C to deal damage to self");

	auto controlDisplay2 = std::make_unique<dae::GameObject>();
	controlDisplay2->GetTransform()->SetLocalPosition({ 60,120,1 });
	controlDisplay2->AddComponent<dae::TextComponent>();
	controlDisplay2->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 10);
	controlDisplay2->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	controlDisplay2->GetComponent<dae::TextComponent>()->SetText("Use D-Pad to move green? tank, A to shoot, B to deal damage to self");

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>()
		->SetText("FPS")
		->SetText("FPS")
		->SetFont("Lingua.otf", 15)
		->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->GetTransform()->SetLocalPosition({ 10, 40,1 });
	scene.Add(std::move(fps));

	scene.Add(std::move(player.Base));
	scene.Add(std::move(player.Turret));
	scene.Add(std::move(tank_2.Base));
	scene.Add(std::move(tank_2.Turret));
	scene.Add(std::move(LivesDisplayTank_1));
	scene.Add(std::move(LivesDisplayTank_2));
	scene.Add(std::move(ScoreDisplay1));
	scene.Add(std::move(ScoreDisplay2));
	scene.Add(std::move(enemyTank_01));
	scene.Add(std::move(enemyTank_02));
	scene.Add(std::move(enemyTank_03));
	scene.Add(std::move(enemyTank_04));
	scene.Add(std::move(enemyTank_05));
	scene.Add(std::move(enemyTank_06));
	scene.Add(std::move(controlDisplay1));
	scene.Add(std::move(controlDisplay2));

}

int main(int, char* []) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "Data/";
	if (!fs::exists(data_location))
		data_location = "./Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);

	return 0;
}
