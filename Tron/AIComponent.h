#ifndef AI_COMP_H
#define AI_COMP_H
#include <memory>

#include "BaseComponent.h"
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
        explicit AIComponent(dae::GameObject* owner) : BaseComponent(owner) {}

		void Update() override;
        void Render() const override {}
        void SetMoveCommand(std::unique_ptr<Tron::MoveCommand> pCommand) { m_pMoveCommand = std::move(pCommand); }
        glm::vec3& GetCurrentDirection() { return m_CurrentDirection; }
    private:
        void ChooseNewDirection();
        bool IsAtTileCenter() const;
        bool ShouldChangeDirection() const;
        bool IsPathBlocked(const glm::vec3& dir) const;
        void SnapToGrid();
        std::unique_ptr<Tron::MoveCommand> m_pMoveCommand;
        glm::vec3 m_CurrentDirection{ 1, 0, 0 }; 
        float m_TileSize{ 32.0f };
        bool m_MadeDecisionThisTile = false;
    };
	};

#endif
