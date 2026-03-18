#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H
#include "BaseComponent.h"
#include "Subject.h"

namespace Tron
{
    class LivesComponent : public dae::BaseComponent
    {
    public:

        LivesComponent(dae::GameObject* owner, int lives) : BaseComponent(owner), m_Lives(lives), m_MaxLives(lives) {}

        LivesComponent(const LivesComponent& other) = delete;
        LivesComponent(LivesComponent&& other) = delete;
        LivesComponent& operator=(LivesComponent&& other) = delete;
        LivesComponent& operator=(const LivesComponent& other) = delete;

        void Update() override {}
        void RenderUI() override {}
        void Render() const override {}


        void DoDamage(int Damage, dae::GameObject* shooter);

        void SetHealth(int newLives);
        void SetMaxHealth(int newMaxLives);
        int GetLives() { return m_Lives; }
        dae::Subject& GetLivesEvent() { return m_LivesEvent; }

    private:
        dae::Subject m_LivesEvent;

        int m_Lives;
        int m_MaxLives;
    };
}

#endif
