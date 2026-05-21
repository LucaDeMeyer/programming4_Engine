#include "States.h"

#include <iostream>

#include "AIComponent.h"
#include "GameManager.h"
#include "TankCommands.h" 
#include "GameTime.h"
#include "InputManager.h"
#include "PlayerComponent.h"
#include "SpriteComponent.h"
#include "LevelManager.h"
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
    manager.LoadLevel(LevelCategory::Menu);
}

std::unique_ptr<Tron::GameState> Tron::MainMenuState::Update(LevelManager& manager)
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
    titleObj->GetTransform()->SetLocalPosition({ 250, 240, 0 });

    int upcomingIndex = manager.GetPlaylistIndex();

    // If we have already cleared levels, we know LoadLevel() WILL increment 
    // the index as soon as this splash screen is over. So we add 1 to predict it.
    if (gameManager.GetTotalLevelsCleared() > 0)
    {
        upcomingIndex++;

        // Wrap around back to level 1 if we reached the end of the 3 CSV files
        if (upcomingIndex >= 3)
        {
            upcomingIndex = 0;
        }
    }

    std::string splashText = "GET READY FOR LEVEL " + std::to_string(upcomingIndex + 1);
    titleObj->AddComponent<dae::TextComponent>()->SetText(splashText)->SetFont("TRON.TTF", 30)->SetColor(0, 255, 255, 255);

    splashScene.Add(std::move(titleObj));

    m_Timer = 0.0f;
}

void Tron::LevelSplashScreenState::OnExit(LevelManager& manager)
{
    dae::SceneManager::GetInstance().GetActiveScene().RemoveAll();
}

std::unique_ptr<Tron::GameState> Tron::LevelSplashScreenState::Update(LevelManager& manager)
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

void Tron::GameplayState::OnExit(LevelManager& manager)
{
}

std::unique_ptr<Tron::GameState> Tron::GameplayState::Update(LevelManager& manager)
{
    return nullptr;
}