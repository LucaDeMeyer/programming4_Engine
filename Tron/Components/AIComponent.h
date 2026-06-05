#ifndef AI_COMP_H
#define AI_COMP_H
#include <memory>

#include "BaseComponent.h"
#include "States.h"
#include "glm/vec3.hpp"
#include "TankCommands.h"

namespace dae
{
	class Command;
}

namespace Tron
{
	struct EnemyProfile;
	enum class AIType;


	class AIComponent : public dae::BaseComponent
	{
	public:
        explicit AIComponent(dae::GameObject* owner,AIType type);
        void Update() override;
        void Render() const override {}

        void SetMoveCommands(std::unique_ptr<MoveCommand> up,std::unique_ptr<MoveCommand> down,std::unique_ptr<MoveCommand> left,std::unique_ptr<MoveCommand> right);
        void SetFireCommand(std::unique_ptr<FireCommand> cmd) { m_pFireCommand = std::move(cmd); }
        glm::vec3& GetCurrentDirection() { return m_CurrentDirection; }

        
        bool IsAtTileCenter() const;
        bool IsPathBlocked(const glm::vec3& dir) const;
        void ChooseNewDirection();
        void SnapToGrid();
        bool CanSeePlayer() const;
        void ChasePlayer();

        void HandlePatrol();
        void HandleChase();
        void HandleAttack();
        std::unique_ptr<EnemyState> GetAggroState() const;

    private:
        void TransitionTo(std::unique_ptr<EnemyState> newState);
        void ExecuteMovement();

        std::unique_ptr<EnemyState> m_CurrentState;

        std::unique_ptr<MoveCommand> m_pMoveUp;
        std::unique_ptr<MoveCommand> m_pMoveDown;
        std::unique_ptr<MoveCommand> m_pMoveLeft;
        std::unique_ptr<MoveCommand> m_pMoveRight;
        std::unique_ptr<FireCommand> m_pFireCommand;
        const EnemyProfile* m_pProfile;

        glm::vec3 m_CurrentDirection{ 1, 0, 0 };
        float m_TileSize{ 32.0f };

        float m_FireCooldown = 1.5f;
        float m_LastFireTime = 0.0f;
        AIType m_Type;

        bool m_MadeDecisionThisTile = false;
    };
	};

#endif
