#ifndef GAME_ACTOR_COMP_H
#define GAME_ACTOR_COMP_H
#include "BaseComponent.h"
#include "Subject.h"
#include "TronEvents.h"
namespace Tron {
    class GameActor : public dae::BaseComponent {
    public:
        explicit GameActor(dae::GameObject* owner) : BaseComponent(owner) {}

        GameActor(const GameActor& other) = delete;
        GameActor(GameActor&& other) = delete;
        GameActor& operator=(GameActor&& other) = delete;
        GameActor& operator=(const GameActor& other) = delete;

        void Update() override {}
        void RenderUI() override {}
        void Render() const override {}

        dae::Subject& GetEventSubject() { return m_Event; }

        int GetActorValue(){ return m_Score; }
        void SetScore(int _score) { m_Score = _score; }

        void InvokeDeath() {
            auto payload = std::make_unique<ActorDied>(GetOwner());
            dae::Event deathEvent(dae::make_sdbm_hash("ActorDied"), std::move(payload));

            m_Event.Notify(GetOwner(), deathEvent);
        }

    private:
        dae::Subject m_Event;
        int m_Score{};
    };
}

#endif