#ifndef AI_COMP_H
#define AI_COMP_H
#include <memory>

#include "BaseComponent.h"
#include "States.h"
#include "glm/vec3.hpp"

namespace Tron
{
	class MoveCommand;
}

namespace dae
{
	class Command;
}

namespace Tron
{
	class AIComponent : public dae::BaseComponent
	{
	public:
        explicit AIComponent(dae::GameObject* owner);
        void Update() override;
        void Render() const override {}

        void SetMoveCommand(std::unique_ptr<MoveCommand> cmd) { m_pMoveCommand = std::move(cmd); }
        glm::vec3& GetCurrentDirection() { return m_CurrentDirection; }

        
        bool IsAtTileCenter() const;
        bool IsPathBlocked(const glm::vec3& dir) const;
        void ChooseNewDirection();
        void SnapToGrid();
        bool CanSeePlayer() const;
        void ChasePlayer();

    private:
        void TransitionTo(EnemyState* newState);

        std::unique_ptr<EnemyState> m_CurrentState;
        std::unique_ptr<MoveCommand> m_pMoveCommand;
        glm::vec3 m_CurrentDirection{ 1, 0, 0 };
        float m_TileSize{ 32.0f };
        bool m_MadeDecisionThisTile = false;
    };
	};

#endif
