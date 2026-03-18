#ifndef TANK_COMMANDS_H
#define TANK_COMMANDS_H
#include "Command.h"
#include "glm/vec2.hpp"

namespace Tron
{
    class MoveCommand final : public dae::ActorCommand
    {
    public:
        MoveCommand(dae::GameObject* obj, const glm::vec2& direction) : dae::ActorCommand(obj), m_Direction(direction) {}
        ~MoveCommand() override = default;
        void Execute() override;

    private:
        glm::vec2 m_Direction;
        void UpdateSpriteDirection();
    };

    class DamageCommand final : public dae::ActorCommand
    {
    public:
        DamageCommand(dae::GameObject* obj, int damage) : dae::ActorCommand(obj), m_Damage(damage) {}
        ~DamageCommand() override = default;
        void Execute() override;

    private:
        int m_Damage;
    };

    class FireCommand final : public dae::ActorCommand
    {
    public:
    	FireCommand(dae::GameObject* obj) : dae::ActorCommand(obj){}
    	~FireCommand() override = default;
        void Execute() override;
    };

    class PlayerFireCommand final : public dae::ActorCommand
    {
    public:
        PlayerFireCommand(dae::GameObject* obj, dae::GameObject* turret) : dae::ActorCommand(obj), m_Turret(turret) {}
        ~PlayerFireCommand() override = default;
        void Execute() override;
    private:
        dae::GameObject* m_Turret;
    };

    class AimCommand final : public dae::ActorCommand
    {
    public:
        AimCommand(dae::GameObject* obj, int controllerIndex = -1) : dae::ActorCommand(obj), m_ControllerIndex(controllerIndex){}
        ~AimCommand() override = default;
        void Execute() override;
    private:
        int m_ControllerIndex;
    };
}
#endif
