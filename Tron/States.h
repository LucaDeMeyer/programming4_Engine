#ifndef STATES_H
#define STATES_H
#include <memory>

#include "Scene.h"
#include "glm/vec3.hpp"

namespace Tron
{
	class LevelManager;

	class PlayerComponent;
	class AIComponent;

    //-----------------
    //Enemy State
    //-----------------
    class EnemyState
    {
    public:
        virtual ~EnemyState() = default;
        virtual void OnEnter(AIComponent& ai) {}
        virtual void OnExit(AIComponent& ai) {}
        virtual std::unique_ptr<EnemyState> Update(AIComponent& ai) = 0;
    };

    class PatrolState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override {}
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    };

    class ChaseState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override;
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    private:
        float m_LostSightTimer = 0.f;
        static constexpr float k_LostSightTimeout = 2.f;
    };

    class AttackState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override {}
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    };

    //-----------------
	//Player State
	//-----------------

    class PlayerState
    {
    public:
        virtual ~PlayerState() = default;
        virtual void OnEnter(PlayerComponent& player) {}
        virtual void OnExit(PlayerComponent& player) {}
        virtual std::unique_ptr<PlayerState> Update(PlayerComponent& player) = 0;
    };


    class NormalPlayerState final : public PlayerState
    {
    public:
        std::unique_ptr<PlayerState> Update(PlayerComponent& player) override;
    };

    class InvulnerableState final : public PlayerState
    {
    public:
        void OnEnter(PlayerComponent& player) override;
        virtual void OnExit(PlayerComponent& player) override;
        std::unique_ptr<PlayerState> Update(PlayerComponent& player) override;
    private:
        float m_Timer = 0.0f;
        static constexpr float k_InvulnerableTime = 3.0f;
    };


    //-----------------
	//Game State
	//-----------------
    class GameState
    {
    public:
        virtual ~GameState() = default;
        virtual void OnEnter(LevelManager& manager) {}
        virtual void OnExit(LevelManager& manager) {}
        virtual std::unique_ptr<GameState> Update(LevelManager& manager) = 0;
    };

    class MainMenuState : public GameState
    {
    public:
        void OnEnter(LevelManager& manager) override;
        std::unique_ptr<GameState> Update(LevelManager& manager) override;
	    
    private:
        struct MenuOption
        {
            dae::GameObject* buttonObj;
            std::function<void()> callback;
        };

        std::vector<MenuOption> m_Options;
        int m_SelectedIndex = 0;

        dae::GameObject* CreateMenuButton(dae::Scene& scene, const std::string& text, float yPos, std::function<void()> callback);

        void UpdateVisuals();
    };

    class LevelSplashScreenState : public GameState
    {
    public:
	    void OnEnter(LevelManager& manager) override;
        void OnExit(LevelManager& manager) override;
        std::unique_ptr<GameState> Update(LevelManager& manager) override;
    private:
        float m_Timer = 0.0f;
        float m_Duration = 3.5f;
    };

    class GameplayState : public GameState
    {
    public:
        void OnEnter(LevelManager& manager) override;
        void OnExit(LevelManager& manager) override;
        std::unique_ptr<GameState> Update(LevelManager& manager) override;
    };

    class HighScoreEntryState final : public GameState
    {
    public:
        void OnEnter(LevelManager& lm) override;
        void OnExit(LevelManager& lm) override;
        std::unique_ptr<GameState> Update(LevelManager& lm) override;

    private:
        // Tracks how many players have confirmed their name
        int  m_ConfirmedCount{ 0 };
        int  m_ExpectedCount{ 1 };   // 1 for single player, 2 for co-op
        bool m_ReadyToLeave{ false };
    };

    class HighScoreScreenState final : public GameState
    {
    public:
        void OnEnter(LevelManager& manager) override;
        void OnExit(LevelManager& manager) override;
        std::unique_ptr<GameState> Update(LevelManager& manager) override;

    private:
        bool m_ShouldLeave{ false };
    };

}
#endif