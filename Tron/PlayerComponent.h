#ifndef PLAYER_COMP_H
#define PLAYER_COMP_H
#include "BaseComponent.h"
#include "States.h"
#include <memory>
#include "glm/vec3.hpp"

namespace Tron
{
    class PlayerComponent : public dae::BaseComponent
    {
    public:
        explicit PlayerComponent(dae::GameObject* owner,int playerIdx);

        void Update() override;
        void Render() const override {}

    
        void TransitionTo(std::unique_ptr<PlayerState> newState);

      
        void SetInvulnerable(bool invulnerable) { m_IsInvulnerable = invulnerable; }
        bool IsInvulnerable() const { return m_IsInvulnerable; }
        void RespawnAtStart();

    private:
        std::unique_ptr<PlayerState> m_CurrentState;
        bool m_IsInvulnerable = false;
        glm::vec3 m_SpawnLocation;
        int m_PlayerIndex;
    };
}
#endif