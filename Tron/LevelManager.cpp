#include "LevelManager.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "TransformComponent.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "AchievmentManager.h"
#include "ButtonComponent.h"
#include "ColliderComponents.h"
#include "Controller.h"
#include "FactionComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "LivesDisplay.h"
#include "ScoreDisplay.h"
#include "SpriteComponent.h"
#include "TankCommands.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TronFactory.h"

void Tron::LevelManager::Init()
{
	auto& menuScene = dae::SceneManager::GetInstance().CreateScene();
	LoadMenu(menuScene);
	dae::SceneManager::GetInstance().SetActiveScene(0);
}

void Tron::LevelManager::LoadLevel(const std::string& path,LevelCategory category)
{
	if (category == LevelCategory::Menu) {
		dae::SceneManager::GetInstance().SetActiveScene(0);
	}
	else {
		auto& gameScene = dae::SceneManager::GetInstance().GetActiveScene();
		gameScene.RemoveAll();
		LoadGrid(path, gameScene);
		dae::SceneManager::GetInstance().SetActiveScene(1);
	}
}

void Tron::LevelManager::LoadGrid(const std::string& path, dae::Scene& scene)
{
    m_Grid.clear();
    m_Rows = 0;
    m_Cols = 0;

    std::ifstream file(path);
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int currentLineCols = 0;

        while (std::getline(ss, cell, ',')) {
            m_Grid.push_back(static_cast<TileType>(std::stoi(cell)));
            currentLineCols++;
        }
        if (m_Cols == 0) m_Cols = currentLineCols;
        m_Rows++;
    }

    float totalLevelWidth = m_Cols * m_TileSize;
    float totalLevelHeight = m_Rows * m_TileSize;

    float windowWidth = 1024.f;
    float windowHeight = 576.f;

    m_OffsetX = (windowWidth - totalLevelWidth) / 2.0f;
    m_OffsetY = (windowHeight - totalLevelHeight) / 2.0f;

    for (size_t i = 0; i < m_Grid.size(); ++i) {
        float x = ((i % m_Cols) * m_TileSize) + m_OffsetX;
        float y = ((i / m_Cols) * m_TileSize) + m_OffsetY;

        auto tile = std::make_unique<dae::GameObject>();
        tile->GetTransform()->SetLocalPosition(glm::vec3{ x, y ,0 });

        std::string texName = GetTextureForType(m_Grid[i]);
        tile->AddComponent<dae::TextureComponent>()->SetTexture(texName);
        tile->GetComponent<dae::TextureComponent>()->SetDimensions(m_TileSize, m_TileSize);
		if (m_Grid[i] == TileType::P1Spawn) {
			m_P1Spawn = { x, y, 1 };
			continue;
		}
		if (m_Grid[i] == TileType::P2Spawn) {
			m_P2Spawn = { x, y, 1 };
			continue;
		}
		if (m_Grid[i] == TileType::EnemySpawn) {
			m_EnemySpawnPoints.emplace_back(glm::vec3{x,y,1});
			continue;
		}
        if (m_Grid[i] == TileType::Wall) {
            tile->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, m_TileSize, m_TileSize});
            tile->AddComponent<FactionComponent>(Team::Wall);
        }
		if (m_Grid[i] == TileType::CenterTile)
		{
			tile->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, m_TileSize, m_TileSize });
			tile->AddComponent<FactionComponent>(Team::Center);
			m_CenterTile = {x,y,1};
		}
		if (m_Grid[i] == TileType::Black || m_Grid[i] == TileType::VerticalPath || m_Grid[i] == TileType::Crossroad || m_Grid[i] == TileType::HorizontalPath) {
			m_EmptyLocations.push_back({ x, y, 1 });
		}

        scene.Add(std::move(tile));
    }


	GameMode currentMode = GameManager::GetInstance().GetGameMode();

	// we should move this into the CVS aswell so we are data driven instead of hard coding all of this
	auto player = Tron::GOFactory::CreatePlayer(m_P1Spawn, "RedTank_SpriteSheet.png", Tron::Team::Player1);

	auto LivesDisplayTank_1 = std::make_unique<dae::GameObject>();
	LivesDisplayTank_1->AddComponent<Tron::LivesDisplay>(player.Base->GetComponent<Tron::LivesComponent>()->GetLives());
	LivesDisplayTank_1->GetComponent<Tron::LivesDisplay>()->SetTexture("Player_Lives.png");
	LivesDisplayTank_1->GetTransform()->SetLocalPosition({ 60, 10, 1 });
	player.Base->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(LivesDisplayTank_1->GetComponent<Tron::LivesDisplay>());

	auto ScoreDisplay1 = std::make_unique<dae::GameObject>();
	ScoreDisplay1->GetTransform()->SetLocalPosition({ 130, 5, 0 });
	ScoreDisplay1->AddComponent<dae::TextComponent>();
	ScoreDisplay1->AddComponent<Tron::ScoreDisplay>();
	player.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(ScoreDisplay1->GetComponent<Tron::ScoreDisplay>());

	player.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(&Tron::AchievementManager::GetInstance());

	auto moveUpCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 0,-100 });
	auto MoveLeftCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ -100,0 });
	auto moveDownCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 0,100 });
	auto MoveRightCommand = std::make_unique<Tron::MoveCommand>(player.Base.get(), glm::vec2{ 100,0 });
	auto fireCommand = std::make_unique<Tron::PlayerFireCommand>(player.Base.get(), player.Turret.get());

	auto damageCommand = std::make_unique<Tron::DamageCommand>(player.Base.get(), 2);
	auto aimCommand1 = std::make_unique<Tron::AimCommand>(player.Turret.get(), -1);

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

	dae::InputManager::GetInstance().BindContinuousCommand(std::move(aimCommand1));

	scene.Add(std::move(player.Base));
	scene.Add(std::move(player.Turret));
	scene.Add(std::move(LivesDisplayTank_1));
	scene.Add(std::move(ScoreDisplay1));

	if (currentMode == GameMode::COOP || currentMode == GameMode::PVP)
	{
		auto tank_2 = Tron::GOFactory::CreatePlayer(m_P2Spawn, "GreenTank_SpriteSheet.png", Tron::Team::Player2);

		auto LivesDisplayTank_2 = std::make_unique<dae::GameObject>();
		LivesDisplayTank_2->AddComponent<Tron::LivesDisplay>(tank_2.Base->GetComponent<Tron::LivesComponent>()->GetLives());
		LivesDisplayTank_2->GetComponent<Tron::LivesDisplay>()->SetTexture("Player_Lives.png");
		LivesDisplayTank_2->GetTransform()->SetLocalPosition({ 600, 10, 1 });
		tank_2.Base->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(LivesDisplayTank_2->GetComponent<Tron::LivesDisplay>());

		auto ScoreDisplay2 = std::make_unique<dae::GameObject>();
		ScoreDisplay2->GetTransform()->SetLocalPosition({ 400, 10, 0 });
		ScoreDisplay2->AddComponent<dae::TextComponent>();
		ScoreDisplay2->AddComponent<Tron::ScoreDisplay>();
		tank_2.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(ScoreDisplay2->GetComponent<Tron::ScoreDisplay>());

		tank_2.Base->GetComponent<Tron::ScoreComponent>()->GetScoreEvent().AddObserver(&Tron::AchievementManager::GetInstance());

		auto moveUpCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{ 0,-100 });
		auto MoveLeftCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{ -100,0 });
		auto moveDownCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{ 0,100 });
		auto MoveRightCommand2 = std::make_unique<Tron::MoveCommand>(tank_2.Base.get(), glm::vec2{ 100,0 });
		auto fireCommand2 = std::make_unique<Tron::PlayerFireCommand>(tank_2.Base.get(), tank_2.Turret.get());

		auto DamageTest = std::make_unique<Tron::DamageCommand>(tank_2.Base.get(), 1);
		auto aimCommand2 = std::make_unique<Tron::AimCommand>(tank_2.Turret.get(), 0);


		dae::InputManager::GetInstance().BindContinuousCommand(std::move(aimCommand2));
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
			0, dae::Controller::ControllerButton::RightShoulder,
			dae::InputState::Down, std::move(fireCommand2));
		dae::InputManager::GetInstance().BindControllerCommand(
			0, dae::Controller::ControllerButton::ButtonB,
			dae::InputState::Down, std::move(DamageTest));

		scene.Add(std::move(tank_2.Base));
		scene.Add(std::move(tank_2.Turret));
		scene.Add(std::move(LivesDisplayTank_2));
		scene.Add(std::move(ScoreDisplay2));
	}
	if (currentMode != GameMode::PVP)
	{
		for (auto& point : m_EnemySpawnPoints)
		{
			auto enemy = Tron::GOFactory::CreateEnemy(point);
			scene.Add(std::move(enemy));
		}
	}

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>()
		->SetText("FPS")
		->SetFont("Lingua.otf", 15)
		->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->GetTransform()->SetLocalPosition({ 10, 40,1 });
	scene.Add(std::move(fps));
}

void Tron::LevelManager::LoadMenu(dae::Scene& scene)
{


	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TextComponent>()
		->SetText("FPS")
		->SetFont("Lingua.otf", 15)
		->SetColor(255, 255, 255, 255);
	fps->AddComponent<dae::FPSComponent>();
	fps->GetTransform()->SetLocalPosition({ 5, 15,1 });
	scene.Add(std::move(fps));

	auto Title = std::make_unique<dae::GameObject>();
	Title->GetTransform()->SetLocalPosition({ 300, 100, 0 });
	Title->AddComponent<dae::TextComponent>()->SetText("TRON - BATTLE TANKS");
	Title->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 25);
	Title->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	scene.Add(std::move(Title));
	// should not be hardcoding these texture sizes and button locations, fine for now since were still testing i guess
  

	auto SinglePlayerBtn = std::make_unique<dae::GameObject>();
	SinglePlayerBtn->GetTransform()->SetLocalPosition({ 400, 250, 0 });
	SinglePlayerBtn->AddComponent<dae::TextComponent>()->SetText("Single Player");
	SinglePlayerBtn->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 25);
	SinglePlayerBtn->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	SinglePlayerBtn->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, 350, 50 });
	auto SinglePlayerBtncomp = SinglePlayerBtn->AddComponent<dae::ButtonComponent>();
	SinglePlayerBtncomp->SetCallback([this]() {
		GameManager::GetInstance().SetGameMode(GameMode::singlePlayer);
		this->LoadLevel("Data/TestLevel.csv", LevelCategory::Game); 
		});
	scene.Add(std::move(SinglePlayerBtn));


	auto COOPBtn = std::make_unique<dae::GameObject>();
	COOPBtn->GetTransform()->SetLocalPosition({ 400, 310, 0 });
	COOPBtn->AddComponent<dae::TextComponent>()->SetText("CO-OP");
	COOPBtn->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 25);
	COOPBtn->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	COOPBtn->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, 200, 50 });
	auto COOPBtncomp = COOPBtn->AddComponent<dae::ButtonComponent>();
	COOPBtncomp->SetCallback([this]() {
		GameManager::GetInstance().SetGameMode(GameMode::COOP);
		this->LoadLevel("Data/TestLevel.csv", LevelCategory::Game);
		});
	scene.Add(std::move(COOPBtn));

	auto PVPbtn = std::make_unique<dae::GameObject>();
	PVPbtn->GetTransform()->SetLocalPosition({ 400, 370, 0 });
	PVPbtn->AddComponent<dae::TextComponent>()->SetText("PVP");
	PVPbtn->GetComponent<dae::TextComponent>()->SetFont("TRON.TTF", 25);
	PVPbtn->GetComponent<dae::TextComponent>()->SetColor(255, 255, 255, 255);
	PVPbtn->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, 200, 50 });
	auto PVPbtncomp = PVPbtn->AddComponent<dae::ButtonComponent>();
	PVPbtncomp->SetCallback([this]() {
		GameManager::GetInstance().SetGameMode(GameMode::PVP);
		this->LoadLevel("Data/TestLevel.csv", LevelCategory::Game);
		});
	scene.Add(std::move(PVPbtn));
}


std::string Tron::LevelManager::GetTextureForType(TileType type) {
    switch (type) {
    case TileType::Wall:			return "Tile_Wall.png";
    case TileType::VerticalPath:	return "Tile_vertical.png";
    case TileType::HorizontalPath:	return "Tile_Horizontal.png";
    case TileType::Crossroad:		return "Tile_Crossing.png";
    case TileType::Black:			return "Tile_Black.png";
    case TileType::P1Spawn:			return "Tile_Black.png";
    case TileType::P2Spawn:			return "Tile_Black.png";
    case TileType::EnemySpawn:		return "Tile_Black.png";
	case TileType::CenterTile:		return "Center_Tile.png";
    default: throw std::runtime_error("Unknown tile type, failed to load level");
    }
}

glm::vec3 Tron::LevelManager::GetRandomPathLocation() {
	int index = rand() % m_EmptyLocations.size();
	return m_EmptyLocations[index];
}

bool Tron::LevelManager::IsWallAt(const glm::vec3& worldPos) const
{
	float totalLevelWidth = m_Cols * m_TileSize;
	float totalLevelHeight = m_Rows * m_TileSize;
	float offsetX = (1024.f - totalLevelWidth) / 2.0f;
	float offsetY = (576.f - totalLevelHeight) / 2.0f;
	int column = static_cast<int>((worldPos.x - offsetX) / m_TileSize);
	int row = static_cast<int>((worldPos.y - offsetY) / m_TileSize);

	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Cols)
		return true; 

	return m_Grid[row * m_Cols + column] == TileType::Wall;
}