#include "LevelManager.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "TransformComponent.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "AchievmentManager.h"
#include "BulletManager.h"

#include "ColliderComponents.h"
#include "Controller.h"
#include "FactionComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "LivesDisplay.h"
#include "ScoreDisplay.h"
#include "ServiceLocator.h"

#include "TankCommands.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TronFactory.h"
#include "ParticleManager.h"
#include "TileMapComponent.h"

void Tron::LevelManager::Init()
{
	auto& sceneManager = dae::SceneManager::GetInstance();

	// Scene 0: Menu
	auto& menuScene = sceneManager.CreateScene();

	//scene 1-3 -> lvls
	sceneManager.CreateScene();
	sceneManager.CreateScene(); 
	sceneManager.CreateScene(); 
	sceneManager.CreateScene();
	sceneManager.CreateScene();
	sceneManager.CreateScene();

	m_CurrentState = std::make_unique<MainMenuState>();
	m_CurrentState->OnEnter(*this);

}

void Tron::LevelManager::TransitionToState(std::unique_ptr<GameState> newState)
{
	m_PendingState = std::move(newState);
}

void Tron::LevelManager::Update()
{
	
	if (m_PendingState)
	{
		if (m_CurrentState) m_CurrentState->OnExit(*this);

		m_CurrentState = std::move(m_PendingState); 

		if (m_CurrentState) m_CurrentState->OnEnter(*this);
	}

	if (m_CurrentState)
	{
		auto nextState = m_CurrentState->Update(*this);
		if (nextState)
		{
			
			TransitionToState(std::move(nextState));
		}
	}
}
void Tron::LevelManager::NextLevel()
{
	TransitionToState(std::make_unique<LevelSplashScreenState>());
}

void Tron::LevelManager::GoToMenu()
{
	TransitionToState(std::make_unique<MainMenuState>());
}

void Tron::LevelManager::LoadLevel(LevelCategory category)
{
	auto& sceneManager = dae::SceneManager::GetInstance();
	auto& inputManager = dae::InputManager::GetInstance();
	auto& gameManager = GameManager::GetInstance();
	auto& audioService = dae::ServiceLocator::GetAudioService();

	audioService.StopAll();

	gameManager.ClearEntities();
	inputManager.ClearAllCommands();
	sceneManager.GetActiveScene().RemoveAll();


	Tron::BulletManager::GetInstance().ClearAll();
	Tron::ParticleManager::GetInstance().ClearAll();

	m_Pplayer1 = nullptr;
	m_Pplayer2 = nullptr;

	if (category == LevelCategory::Game)
	{
		size_t nextSceneIdx = m_CurrentLevelIndex + 1;
		if (nextSceneIdx > 3)
			nextSceneIdx = 1;

		if (m_CurrentLevelIndex != 0)
		{
			m_LevelPlaylistIndex++;
			if (m_LevelPlaylistIndex >= static_cast<int>(m_LevelFiles.size()))
				m_LevelPlaylistIndex = 0;
		}

		m_CurrentLevelIndex = nextSceneIdx;
		sceneManager.SetActiveScene(nextSceneIdx);

		auto& activeScene = sceneManager.GetActiveScene();
		activeScene.RemoveAll();

		std::string actualFile = m_LevelFiles[m_LevelPlaylistIndex];
		LoadGrid(actualFile, activeScene);

	
		gameManager.SetTransitioning(false);
	}
}

void Tron::LevelManager::LoadGrid( std::string& path, dae::Scene& scene)
{
	ParseGrid(path, scene);
	SpawnPlayers(scene);
	SpawnEnemies(scene);
	CreateFPSCounter(scene, { 10, 40, 1 });
	SetupLevelAudio();
}

void Tron::LevelManager::ParseGrid(std::string& path, dae::Scene& scene)
{
	m_Grid.clear();
	m_Rows = 0;
	m_Cols = 0;
	m_TankSpawnPoints.clear();
	m_RecogniserSpawnPoints.clear();
	m_EmptyLocations.clear();

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
	//use flyweight pattern to make the map instead of individual GOs per tile
	auto mapObject = std::make_unique<dae::GameObject>();
	auto tileMap = mapObject->AddComponent<TileMapComponent>(m_TileSize);
	tileMap->LoadLevel(path);

	float totalLevelWidth = m_Cols * m_TileSize;
	float totalLevelHeight = m_Rows * m_TileSize;
	auto winSize = GameManager::GetInstance().GetWindowSize();
	m_OffsetX = (winSize.x - totalLevelWidth) / 2.0f;
	m_OffsetY = (winSize.y - totalLevelHeight) / 2.0f;
	mapObject->GetTransform()->SetLocalPosition({ m_OffsetX, m_OffsetY, 0 });

	scene.Add(std::move(mapObject));

	for (size_t i = 0; i < m_Grid.size(); ++i) {
		float x = ((i % m_Cols) * m_TileSize) + m_OffsetX;
		float y = ((i / m_Cols) * m_TileSize) + m_OffsetY;
		glm::vec3 pos{ x, y, 1 };

		switch (m_Grid[i]) {
		case TileType::P1Spawn:      m_P1Spawn = pos; break;
		case TileType::P2Spawn:      m_P2Spawn = pos; break;
		case TileType::RecogniserSpawn: m_RecogniserSpawnPoints.push_back(pos); break;
		case TileType::TankSpawn:   m_TankSpawnPoints.push_back(pos); break;
		case TileType::CenterTile:
		{
			auto centerTile = std::make_unique<dae::GameObject>();
			centerTile->GetTransform()->SetLocalPosition(pos);
			centerTile->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, m_TileSize, m_TileSize });
			centerTile->AddComponent<FactionComponent>(Team::Center);
			centerTile->AddComponent<dae::TextureComponent>()->SetTexture("Center_Tile.png");
			m_CenterTile = pos;
			scene.Add(std::move(centerTile));
			break;
		}
		case TileType::Wall:
		{
			auto wallTile = std::make_unique<dae::GameObject>();
			wallTile->GetTransform()->SetLocalPosition(pos);
			wallTile->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, m_TileSize, m_TileSize });
			wallTile->AddComponent<FactionComponent>(Team::Wall);
			scene.Add(std::move(wallTile));
			break;
		}
		case TileType::Black:
		case TileType::VerticalPath:
		case TileType::Crossroad:
		case TileType::HorizontalPath:
			m_EmptyLocations.push_back(pos);
			break;
		}
	}

}
void Tron::LevelManager::SpawnPlayers( dae::Scene& scene)
{
	GameMode currentGameMode = GameManager::GetInstance().GetGameMode();

	SpawnSinglePlayer(scene, 0, m_P1Spawn, "RedTank_SpriteSheet.png", Tron::Team::Player1);

	if (currentGameMode == GameMode::COOP || currentGameMode == GameMode::PVP)
	{
		Team p2Team = (currentGameMode == GameMode::PVP) ? Team::Enemy : Team::Player1;
		SpawnSinglePlayer(scene, 1, m_P2Spawn, "GreenTank_SpriteSheet.png", p2Team);
	}
}

void Tron::LevelManager::SpawnSinglePlayer( dae::Scene& scene, int playerIndex, const glm::vec3& spawnPos, const std::string& texture, Tron::Team team)
{
	auto player = Tron::GOFactory::CreatePlayer(spawnPos, texture, team,playerIndex);
	auto* pTankBase = player.Base.get();
	auto Explosion = std::make_unique<dae::GameObject>();
	if (playerIndex == 0) m_Pplayer1 = pTankBase;
	else m_Pplayer2 = pTankBase;

	auto winSize = GameManager::GetInstance().GetWindowSize();
	float uiXOffset = (playerIndex == 0) ? winSize.x * 0.05f : winSize.x * 0.75f;
	float scoreXOffset = (playerIndex == 0) ? winSize.x * 0.15f : winSize.x * 0.85f;
	float uiYPos = winSize.y * 0.02f;

	auto livesDisplay = std::make_unique<dae::GameObject>();
	livesDisplay->GetTransform()->SetLocalPosition({ uiXOffset, uiYPos, 1 });
	auto livesComp = livesDisplay->AddComponent<Tron::LivesDisplay>(pTankBase->GetComponent<Tron::LivesComponent>()->GetLives());
	livesComp->SetTexture("Player_Lives.png");
	pTankBase->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(livesComp);
	pTankBase->GetComponent<Tron::LivesComponent>()->GetLivesEvent().AddObserver(&GameManager::GetInstance());

	auto scoreDisplay = std::make_unique<dae::GameObject>();
	scoreDisplay->GetTransform()->SetLocalPosition({ scoreXOffset, 5, 0 });
	scoreDisplay->AddComponent<dae::TextComponent>();
	scoreDisplay->AddComponent<Tron::ScoreDisplay>();

	auto scoreEvent = &pTankBase->GetComponent<Tron::ScoreComponent>()->GetScoreEvent();
	scoreEvent->AddObserver(scoreDisplay->GetComponent<Tron::ScoreDisplay>());
	scoreEvent->AddObserver(&Tron::AchievementManager::GetInstance());
	scoreEvent->AddObserver(&GameManager::GetInstance());


	if (auto scoreComp = pTankBase->GetComponent<ScoreComponent>()) {
		int previousScore = (playerIndex == 0) ? GameManager::GetInstance().m_P1Score : GameManager::GetInstance().m_p2Score;
		scoreComp->AddScore(previousScore);
		scoreComp->SetPlayerIndex(playerIndex);
	}

	if (auto livesComp = pTankBase->GetComponent<LivesComponent>()) {
		int previousScore = (playerIndex == 0) ? GameManager::GetInstance().m_P1Lives : GameManager::GetInstance().m_P2Lives;
		livesComp->SetPlayerID(playerIndex);
		livesComp->SetHealth(previousScore);
	}

	auto& input = dae::InputManager::GetInstance();

	int numControllers = input.GetConnectedControllerCount();
	int controllerIndex = playerIndex;

	GameMode currentGameMode = GameManager::GetInstance().GetGameMode();
	if ((currentGameMode == GameMode::COOP || currentGameMode == GameMode::PVP) && numControllers == 1)
	{
		if (playerIndex == 0) controllerIndex = -1; 
		if (playerIndex == 1) controllerIndex = 0;
	}

	if (playerIndex == 0) {
		input.RegisterMovementCommand(SDLK_W, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 0,-100 }));
		input.RegisterMovementCommand(SDLK_S, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 0,100 }));
		input.RegisterMovementCommand(SDLK_A, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ -100,0 }));
		input.RegisterMovementCommand(SDLK_D, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 100,0 }));
		input.BindKeyCommand(SDLK_SPACE, dae::InputState::Down, std::make_unique<Tron::PlayerFireCommand>(pTankBase, player.Turret.get()));
	


		input.BindKeyCommand(SDLK_UP, dae::InputState::Pressed, std::make_unique<Tron::PlayerAimCommand>(pTankBase,player.Turret.get(), glm::vec2{ 0,-1 }));
		input.BindKeyCommand(SDLK_DOWN, dae::InputState::Pressed, std::make_unique<Tron::PlayerAimCommand>(pTankBase,player.Turret.get(), glm::vec2{ 0,1 }));
		input.BindKeyCommand(SDLK_LEFT, dae::InputState::Pressed, std::make_unique<Tron::PlayerAimCommand>(pTankBase,player.Turret.get(), glm::vec2{ -1,0 }));
		input.BindKeyCommand(SDLK_RIGHT, dae::InputState::Pressed, std::make_unique<Tron::PlayerAimCommand>(pTankBase,player.Turret.get(), glm::vec2{ 1,0 }));

		input.BindKeyCommand(SDLK_F1, dae::InputState::Down, std::make_unique<Tron::SkipLevelCommand>());
		input.BindKeyCommand(SDLK_F2, dae::InputState::Down, std::make_unique<Tron::ToggleMuteCommand>());
		//input.BindContinuousCommand(std::make_unique<Tron::AimCommand>(player.Turret.get(), -1)); // -1 for Keyboard
	}

	if (controllerIndex != -1) {
		input.BindContinuousCommand(std::make_unique<Tron::AimCommand>(player.Turret.get(), controllerIndex));
		input.RegisterControllerMovementCommand(controllerIndex, dae::Controller::ControllerButton::DPadUp, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 0,-100 }));
		input.RegisterControllerMovementCommand(controllerIndex, dae::Controller::ControllerButton::DPadDown, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 0,100 }));
		input.RegisterControllerMovementCommand(controllerIndex, dae::Controller::ControllerButton::DPadLeft, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ -100,0 }));
		input.RegisterControllerMovementCommand(controllerIndex, dae::Controller::ControllerButton::DPadRight, std::make_unique<Tron::PlayerMoveCommand>(pTankBase, glm::vec2{ 100,0 }));
		input.BindControllerCommand(controllerIndex, dae::Controller::ControllerButton::RightShoulder, dae::InputState::Down, std::make_unique<Tron::PlayerFireCommand>(pTankBase, player.Turret.get()));
		input.BindControllerCommand(controllerIndex, dae::Controller::ControllerButton::ButtonB, dae::InputState::Down, std::make_unique<Tron::DamageCommand>(pTankBase, 2)); // Debug damage
	}

	scene.Add(std::move(player.Base));
	scene.Add(std::move(player.Turret));
	scene.Add(std::move(livesDisplay));
	scene.Add(std::move(scoreDisplay));
	//scene.Add(std::move(Explosion));
}

void Tron::LevelManager::SpawnEnemies(dae::Scene& scene)
{
	if (GameManager::GetInstance().GetGameMode() == GameMode::PVP) return;

	for (auto& point : m_TankSpawnPoints) {

		auto enemy = Tron::GOFactory::CreateEnemy(point, Tron::AIType::Tank);
		scene.Add(std::move(enemy));
	}

	for (auto& point : m_RecogniserSpawnPoints) {
		auto enemy = Tron::GOFactory::CreateEnemy(point, Tron::AIType::Recogniser);
		auto Explosion = std::make_unique<dae::GameObject>();
		scene.Add(std::move(enemy));
	}

	int round = GameManager::GetInstance().GetTotalLevelsCleared();

	int extraTanks = round / 2; // extra tank every 2 lvls
	int extraRecognisers = round / 3; // extra recogniser every 3 lvls

	for (int i = 0; i < extraTanks; ++i) {
		scene.Add(Tron::GOFactory::CreateEnemy(GetRandomPathLocation(), Tron::AIType::Tank));
	}
	for (int i = 0; i < extraRecognisers; ++i) {
		scene.Add(Tron::GOFactory::CreateEnemy(GetRandomPathLocation(), Tron::AIType::Recogniser));
	}
}

void Tron::LevelManager::SetupLevelAudio()
{
	auto& audioService = dae::ServiceLocator::GetAudioService();
	audioService.LoadSound(dae::Utils::make_sdbm_hash("tank_fire"), "Data/Tank_Fire.wav");

	audioService.LoadSound(dae::Utils::make_sdbm_hash("Bullet_Explosion"), "Data/Bullet_Explosion.wav");
	audioService.LoadSound(dae::Utils::make_sdbm_hash("Tank_Explosion"), "Data/Tank_Explosion.wav");
	audioService.LoadSound(dae::Utils::make_sdbm_hash("Level_Theme"), "Data/Level_Temp_Music.wav");
	audioService.Play(dae::Utils::make_sdbm_hash("Level_Theme"), 1.0f, dae::AudioType::Ambient);
}

void Tron::LevelManager::CreateFPSCounter(dae::Scene& scene, const glm::vec3& pos)
{
	auto fps = std::make_unique<dae::GameObject>();
	fps->GetTransform()->SetLocalPosition(pos);
	fps->AddComponent<dae::TextComponent>()->SetFont("Lingua.otf", 15)->SetColor(255, 255, 255, 255)->SetText("FPS");
	fps->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(fps));
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
    case TileType::TankSpawn:		return "Tile_Black.png";
	case TileType::CenterTile:		return "Center_Tile.png";
    case TileType::RecogniserSpawn: return "Tile_Black.png";
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

	auto winSize = GameManager::GetInstance().GetWindowSize();
	float offsetX = (winSize.x - totalLevelWidth) / 2.0f;
	float offsetY = (winSize.y - totalLevelHeight) / 2.0f;
	int column = static_cast<int>((worldPos.x - offsetX + .5f) / m_TileSize);
	int row = static_cast<int>((worldPos.y - offsetY + .5f) / m_TileSize);

	if (row < 0 || row >= m_Rows || column < 0 || column >= m_Cols)
		return true;

	return m_Grid[row * m_Cols + column] == TileType::Wall;
}


dae::GameObject* Tron::LevelManager::GetNearestPlayer(const glm::vec3& pos) const
{
	dae::GameObject* nearest = nullptr;
	float bestDist = std::numeric_limits<float>::max();

	if (m_Pplayer1 && !m_Pplayer1->IsMarkedForDestruction())
	{
		auto* pTrans = m_Pplayer1->GetTransform(); 
		if (pTrans)
		{
			float d = glm::distance(pTrans->GetLocalPosition(), pos);
			if (d < bestDist) { bestDist = d; nearest = m_Pplayer1; }
		}
	}
	if (m_Pplayer2 && !m_Pplayer2->IsMarkedForDestruction())
	{
		auto* pTrans = m_Pplayer2->GetTransform();
		if (pTrans)
		{
			float d = glm::distance(pTrans->GetLocalPosition(), pos);
			if (d < bestDist) { bestDist = d; nearest = m_Pplayer2; }
		}
	}

	return nearest;
}