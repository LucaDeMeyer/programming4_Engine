#ifndef TANK_COMMANDS_H
#define TANK_COMMANDS_H
#include "Command.h"
#include "glm/vec2.hpp"

class MoveCommand final : public dae::ActorCommand
{
public:
    MoveCommand(dae::GameObject* obj, const glm::vec2& direction) : dae::ActorCommand(obj), m_Direction(direction){}
    ~MoveCommand() override = default;
    void Execute() override;

private:
    glm::vec2 m_Direction;
};
#endif
