#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H
#include "BaseComponent.h"
#include "Subject.h"

namespace Tron
{
    class ScoreComponent : public dae::BaseComponent
    {
    public:

        ScoreComponent(dae::GameObject* owner) : BaseComponent(owner), m_Score(0){}

        ScoreComponent(const ScoreComponent& other) = delete;
        ScoreComponent(ScoreComponent&& other) = delete;
        ScoreComponent& operator=(ScoreComponent&& other) = delete;
        ScoreComponent& operator=(const ScoreComponent& other) = delete;

        void Update() override {}
        void RenderUI() override {}
        void Render() const override {}


        void AddScore(int Score);
        int GetScore() { return m_Score; }
        dae::Subject& GetScoreEvent() { return m_ScoreEvent; }

    private:
        dae::Subject m_ScoreEvent;
        int m_Score;
      
    };
}

#endif
