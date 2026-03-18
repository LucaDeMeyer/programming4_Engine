#ifndef FACTION_COMPONENT_H
#define FACTION_COMPONENT_H
#include "BaseComponent.h"

namespace Tron
{
    enum class Team
    {
        Player1,
        Player2,
        Enemy,
        Neutral
    };
    class FactionComponent final : public dae::BaseComponent
    {
    public:
        explicit FactionComponent(dae::GameObject* owner, Team team)
            : BaseComponent(owner), m_Team(team) {
        }

        Team GetTeam() const { return m_Team; }
        void SetTeam(Team team) { m_Team = team; }

        void Update() override {}
        void Render() const override {}

    private:
        Team m_Team;
    };
}
#endif