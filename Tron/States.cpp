#include "States.h"

#include <iostream>

#include "AIComponent.h"
#include "ButtonComponent.h"
#include "ColliderComponents.h"
#include "FPSComponent.h"
#include "GameManager.h"
#include "TankCommands.h" 
#include "GameTime.h"
#include "InputManager.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include "LevelManager.h"
#include "NameEntryCommands.h"
#include "NameEntryComponent.h"
#include "NameEntryDisplay.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "TextComponent.h"
#include "TransformComponent.h"

std::unique_ptr<Tron::EnemyState> Tron::PatrolState::Update(AIComponent& ai)
{
   
    if (ai.CanSeePlayer())
    {
        
        return ai.GetAggroState();
    }

    ai.HandlePatrol();

    return nullptr;
}
void Tron::ChaseState::OnEnter(AIComponent& ai)
{
    m_LostSightTimer = 0.f;
}

std::unique_ptr<Tron::EnemyState> Tron::ChaseState::Update(AIComponent& ai)
{
    if (!ai.CanSeePlayer())
    {
        m_LostSightTimer += Time::GetInstance().GetDeltaTime();
        if (m_LostSightTimer >= k_LostSightTimeout)
        {
            return std::make_unique<PatrolState>(); 
        }
    }
    else
    {
        m_LostSightTimer = 0.f;
    }

  
    ai.HandleChase();

    return nullptr;
}
std::unique_ptr<Tron::EnemyState> Tron::AttackState::Update(AIComponent& ai)
{
    if (!ai.CanSeePlayer())
    {
        return std::make_unique<PatrolState>();
    }

    ai.HandlePatrol(); 
    ai.HandleAttack();

    return nullptr;
}

std::unique_ptr<Tron::PlayerState> Tron::NormalPlayerState::Update(PlayerComponent& player)
{
    return nullptr;
}


void Tron::InvulnerableState::OnEnter(PlayerComponent& player)
{
    m_Timer = 0.0f;
    player.SetInvulnerable(true);
    player.RespawnAtStart();
}

std::unique_ptr<Tron::PlayerState> Tron::InvulnerableState::Update(PlayerComponent& player)
{
    float dt = Time::GetInstance().GetDeltaTime();
    m_Timer += dt;

    
    auto* sprite = player.GetOwner()->GetComponent<dae::SpriteComponent>();
    if (sprite)
    {
        
        bool isVisible = (static_cast<int>(m_Timer * 10) % 2) == 0;
        sprite->SetVisible(isVisible); 
    }

    if (m_Timer >= k_InvulnerableTime)
    {
        return std::make_unique<NormalPlayerState>();
    }

    return nullptr;
}

void Tron::InvulnerableState::OnExit(PlayerComponent& player)
{
    player.SetInvulnerable(false);

    auto* sprite = player.GetOwner()->GetComponent<dae::SpriteComponent>();
    if (sprite)
    {
        sprite->SetVisible(true);
    }
}

void Tron::MainMenuState::OnEnter(LevelManager& manager)
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputManager = dae::InputManager::GetInstance();
    auto& gameManager = GameManager::GetInstance();
    auto& audioService = dae::ServiceLocator::GetAudioService();

    sceneManager.SetActiveScene(0);
    auto& scene = sceneManager.GetActiveScene();
    scene.RemoveAll();

    audioService.StopAll();
    gameManager.ClearEntities();
    inputManager.ClearAllCommands();
    gameManager.SetTransitioning(false);

    audioService.LoadSound(dae::Utils::make_sdbm_hash("Theme_Music"), "Data/TronMenu_Theme.wav");
    audioService.Play(dae::Utils::make_sdbm_hash("Theme_Music"), 1.0f, dae::AudioType::Ambient);

    glm::vec2 winSize = gameManager.GetWindowSize();
    float centerX = winSize.x / 2.0f;

    auto fps = std::make_unique<dae::GameObject>();
    fps->GetTransform()->SetLocalPosition({ 5.f, 15.f, 1.f });
    fps->AddComponent<dae::TextComponent>()->SetFont("Lingua.otf", 15)->SetColor(255, 255, 255, 255)->SetText("FPS");
    fps->AddComponent<dae::FPSComponent>();
    scene.Add(std::move(fps));

    auto title = std::make_unique<dae::GameObject>();
    title->AddComponent<dae::TextComponent>()->SetFont("TRON.TTF", 25)->SetColor(255, 255, 255, 255)->SetText("TRON - BATTLE TANKS");
    glm::vec2 texSize = title->GetComponent<dae::TextComponent>()->GetTexture()->GetSize();

    title->GetTransform()->SetLocalPosition({ centerX - texSize.x/2, 100.f, 0.f });
    scene.Add(std::move(title));

    float startY = 250.0f;
    float spacing = 60.0f;

    CreateMenuButton(scene, "Single Player", startY, [&manager]() {
        GameManager::GetInstance().SetGameMode(GameMode::singlePlayer);
        manager.TransitionToState(std::make_unique<LevelSplashScreenState>());
        });

    CreateMenuButton(scene, "CO-OP", startY + spacing, [&manager]() {
        GameManager::GetInstance().SetGameMode(GameMode::COOP);
        manager.TransitionToState(std::make_unique<LevelSplashScreenState>());
        });

    CreateMenuButton(scene, "PVP", startY + (spacing * 2), [&manager]() {
        GameManager::GetInstance().SetGameMode(GameMode::PVP);
        manager.TransitionToState(std::make_unique<LevelSplashScreenState>());
        });
}

void Tron::MainMenuState::CreateMenuButton(dae::Scene& scene, const std::string& text, float yPos, std::function<void()> callback)
{
    auto btnObj = std::make_unique<dae::GameObject>();
    auto textComp = btnObj->AddComponent<dae::TextComponent>();
    textComp->SetFont("TRON.TTF", 25)
	->SetColor(255, 255, 255, 255)
	->SetText(text);

    glm::vec2 texSize = textComp->GetTexture()->GetSize();
    float windowWidth = GameManager::GetInstance().GetWindowSize().x;
    float centerX = (windowWidth / 2.0f) - (texSize.x / 2.0f);
    btnObj->GetTransform()->SetLocalPosition({ centerX, yPos, 1 });

    btnObj->AddComponent<dae::BoxColliderComponent>(glm::vec4{ 0, 0, texSize.x, texSize.y });

    auto btnComp = btnObj->AddComponent<dae::ButtonComponent>();
    btnComp->SetCallback(callback);
    scene.Add(std::move(btnObj));
}


std::unique_ptr<Tron::GameState> Tron::MainMenuState::Update(LevelManager&)
{
    return nullptr;
}

void Tron::LevelSplashScreenState::OnEnter(LevelManager& manager)
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputManager = dae::InputManager::GetInstance();
    auto& gameManager = GameManager::GetInstance();
    auto& audioService = dae::ServiceLocator::GetAudioService();

    audioService.StopAll();
    gameManager.ClearEntities();
    inputManager.ClearAllCommands();

    sceneManager.GetActiveScene().RemoveAll();

    sceneManager.SetActiveScene(4);
    auto& splashScene = sceneManager.GetActiveScene();
    splashScene.RemoveAll();

    auto titleObj = std::make_unique<dae::GameObject>();
    int upcomingIndex = manager.GetPlaylistIndex();

    if (gameManager.GetTotalLevelsCleared() > 0)
    {
        upcomingIndex++;

        if (upcomingIndex >= 3)
        {
            upcomingIndex = 0;
        }
    }
    std::string splashText = "GET READY FOR LEVEL " + std::to_string(upcomingIndex + 1);
    titleObj->AddComponent<dae::TextComponent>()->SetFont("TRON.TTF", 30)->SetColor(0, 255, 255, 255)->SetText(splashText);

    glm::vec2 pos = { gameManager.GetWindowSize().x/2 - titleObj->GetComponent<dae::TextComponent>()->GetTexture()->GetSize().x/2,gameManager.GetWindowSize().y / 2 - titleObj->GetComponent<dae::TextComponent>()->GetTexture()->GetSize().y/2 };
    titleObj->GetTransform()->SetLocalPosition({ pos.x,pos.y,1 });
    splashScene.Add(std::move(titleObj));

    m_Timer = 0.0f;
}

void Tron::LevelSplashScreenState::OnExit(LevelManager&)
{
    dae::SceneManager::GetInstance().GetActiveScene().RemoveAll();
}

std::unique_ptr<Tron::GameState> Tron::LevelSplashScreenState::Update(LevelManager&)
{
    float dt = Time::GetInstance().GetDeltaTime();
    m_Timer += dt;
    if (m_Timer >= m_Duration)
    {
        return std::make_unique<GameplayState>();
    }

    return nullptr;
}

void Tron::GameplayState::OnEnter(LevelManager& manager)
{
    manager.LoadLevel(LevelCategory::Game);
}

void Tron::GameplayState::OnExit(LevelManager&)
{
}

std::unique_ptr<Tron::GameState> Tron::GameplayState::Update(LevelManager&)
{
    return nullptr;
}
void Tron::HighScoreEntryState::OnEnter(LevelManager&)
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputManager = dae::InputManager::GetInstance();
    auto& gameManager = GameManager::GetInstance();

    inputManager.ClearAllCommands();
    gameManager.ClearEntities();

    sceneManager.SetActiveScene(4);
    auto& scene = sceneManager.GetActiveScene();
    scene.RemoveAll();

    GameMode mode = gameManager.GetGameMode();
    m_ExpectedCount = (mode == GameMode::COOP) ? 2 : 1;
    m_ConfirmedCount = 0;
    m_ReadyToLeave = false;


    {
        auto obj = std::make_unique<dae::GameObject>();
        obj->GetTransform()->SetLocalPosition({ 330.f, 60.f, 0.f });
        obj->AddComponent<dae::TextComponent>()
            ->SetFont("TRON.TTF", 22)
            ->SetText("ENTER YOUR INITIALS")
            ->SetColor(0, 200, 255, 255);
        scene.Add(std::move(obj));
    }

    {
        auto obj = std::make_unique<dae::GameObject>();
        obj->GetTransform()->SetLocalPosition({ 240.f, 460.f, 0.f });
        obj->AddComponent<dae::TextComponent>()
            ->SetFont("TRON.TTF", 14)
            ->SetText("UP/DOWN: scroll   LEFT/RIGHT: move   FIRE/A: confirm")
            ->SetColor(120, 120, 120, 255);
        scene.Add(std::move(obj));
    }

   
    auto spawnEntry = [&](int playerIndex, glm::vec3 origin, const std::string& label)
        {
            auto entryObj = std::make_unique<dae::GameObject>();
            auto* raw = entryObj.get();

            auto* nameEntry = raw->AddComponent<NameEntryComponent>(playerIndex);

            nameEntry->SetOnConfirmed([this, &gameManager](int pIdx, const std::string& name)
                {
                    int score = (pIdx == 0) ? gameManager.m_P1Score : gameManager.m_p2Score;
                    gameManager.AddScore(name, score);
                    ++m_ConfirmedCount;
                });

          
            auto* display = raw->AddComponent<NameEntryDisplay>(nameEntry, &scene, origin, label);

            scene.Add(std::move(entryObj));

            display->Init();

            // ---- Keyboard (Player 1 only — arrow keys + Enter/Space) ----
            if (playerIndex == 0)
            {
                inputManager.BindKeyCommand(SDLK_UP, dae::InputState::Down,
                    std::make_unique<ScrollUpCommand>(raw, nameEntry));
                inputManager.BindKeyCommand(SDLK_DOWN, dae::InputState::Down,
                    std::make_unique<ScrollDownCommand>(raw, nameEntry));
                inputManager.BindKeyCommand(SDLK_LEFT, dae::InputState::Down,
                    std::make_unique<SlotLeftCommand>(raw, nameEntry));
                inputManager.BindKeyCommand(SDLK_RIGHT, dae::InputState::Down,
                    std::make_unique<SlotRightCommand>(raw, nameEntry));
                inputManager.BindKeyCommand(SDLK_RETURN, dae::InputState::Down,
                    std::make_unique<ConfirmNameCommand>(raw, nameEntry));
                inputManager.BindKeyCommand(SDLK_SPACE, dae::InputState::Down,
                    std::make_unique<ConfirmNameCommand>(raw, nameEntry));
            }

          
            unsigned int ci = static_cast<unsigned int>(playerIndex);

            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::DPadUp, dae::InputState::Down,
                std::make_unique<ScrollUpCommand>(raw, nameEntry));
            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::DPadDown, dae::InputState::Down,
                std::make_unique<ScrollDownCommand>(raw, nameEntry));
            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::DPadLeft, dae::InputState::Down,
                std::make_unique<SlotLeftCommand>(raw, nameEntry));
            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::DPadRight, dae::InputState::Down,
                std::make_unique<SlotRightCommand>(raw, nameEntry));
            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::RightShoulder, dae::InputState::Down,
                std::make_unique<ConfirmNameCommand>(raw, nameEntry));
            inputManager.BindControllerCommand(ci,
                dae::Controller::ControllerButton::ButtonA, dae::InputState::Down,
                std::make_unique<ConfirmNameCommand>(raw, nameEntry));
        };

  
    spawnEntry(0, { 280.f, 220.f, 0.f }, "PLAYER 1");

    if (mode == GameMode::COOP)
        spawnEntry(1, { 580.f, 220.f, 0.f }, "PLAYER 2");
}

void Tron::HighScoreEntryState::OnExit(LevelManager&)
{
   
}

std::unique_ptr<Tron::GameState> Tron::HighScoreEntryState::Update(LevelManager&)
{
    if (!m_ReadyToLeave && m_ConfirmedCount >= m_ExpectedCount)
    {
        m_ReadyToLeave = true;

        auto& gm = GameManager::GetInstance();
        gm.m_P1Score = 0;
        gm.m_p2Score = 0;
        gm.m_LVLNR = 0;

        return std::make_unique<HighScoreScreenState>();
    }
    return nullptr;
}

void Tron::HighScoreScreenState::OnEnter(LevelManager&)
{
    auto& sceneManager = dae::SceneManager::GetInstance();
    auto& inputManager = dae::InputManager::GetInstance();
    auto& gm = GameManager::GetInstance();

    inputManager.ClearAllCommands();

    sceneManager.SetActiveScene(5);
    auto& scene = sceneManager.GetActiveScene();
    scene.RemoveAll();


    bool* shouldLeave = &m_ShouldLeave;

    auto confirmCallback = [shouldLeave]()
        {
            *shouldLeave = true;
        };

    // not sure if we should allow both controllers to handle this, is just a matter of adding extra command with diffrent index
    inputManager.BindKeyCommand(SDLK_RETURN,
        dae::InputState::Down,
        std::make_unique<ConfirmCommand>(nullptr, confirmCallback));

    inputManager.BindKeyCommand(SDLK_SPACE,
        dae::InputState::Down,
        std::make_unique<ConfirmCommand>(nullptr, confirmCallback));


    inputManager.BindControllerCommand(
        0,
        dae::Controller::ControllerButton::ButtonA,
        dae::InputState::Down,
        std::make_unique<ConfirmCommand>(nullptr, confirmCallback));

    inputManager.BindControllerCommand(
        0,
        dae::Controller::ControllerButton::RightShoulder,
        dae::InputState::Down,
        std::make_unique<ConfirmCommand>(nullptr, confirmCallback));
   
    {
        auto obj = std::make_unique<dae::GameObject>();
        obj->GetTransform()->SetLocalPosition({ 360.f, 50.f, 0.f });

        obj->AddComponent<dae::TextComponent>()
            ->SetFont("TRON.TTF", 28)
            ->SetText("HIGH SCORES")
            ->SetColor(0, 255, 255, 255);

        scene.Add(std::move(obj));
    }

    float startY = 140.f;

    const auto& scores = gm.GetHighScores();

    for (size_t i = 0; i < scores.size(); ++i)
    {
        const auto& entry = scores[i];

        auto obj = std::make_unique<dae::GameObject>();
        obj->GetTransform()->SetLocalPosition({ 320.f, startY + i * 40.f, 0.f });

        std::string text =
            std::to_string(i + 1) + ". " +
            entry.name + "  -  " +
            std::to_string(entry.score);

        obj->AddComponent<dae::TextComponent>()
            ->SetFont("TRON.TTF", 20)
            ->SetText(text)
            ->SetColor(255, 255, 255, 255);

        scene.Add(std::move(obj));
    }

    // Continue text
    {
        auto obj = std::make_unique<dae::GameObject>();
        obj->GetTransform()->SetLocalPosition({ 260.f, 500.f, 0.f });

        obj->AddComponent<dae::TextComponent>()
            ->SetFont("TRON.TTF", 16)
            ->SetText("PRESS ENTER OR A TO RETURN TO MENU")
            ->SetColor(180, 180, 180, 255);

        scene.Add(std::move(obj));
    }


}

void Tron::HighScoreScreenState::OnExit(LevelManager&)
{
    dae::SceneManager::GetInstance().GetActiveScene().RemoveAll();
}

std::unique_ptr<Tron::GameState>
Tron::HighScoreScreenState::Update(LevelManager&)
{
  

    if (m_ShouldLeave)
    {
        auto& gm = GameManager::GetInstance();

        gm.m_P1Score = 0;
        gm.m_p2Score = 0;
        gm.m_LVLNR = 0;

        return std::make_unique<MainMenuState>();
    }

    return nullptr;
}