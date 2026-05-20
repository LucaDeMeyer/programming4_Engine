#ifndef STATES_H
#define STATES_H
#include <memory>

namespace Tron
{
    class AIComponent;

    class EnemyState
    {
    public:
        virtual ~EnemyState() = default;
        virtual void OnEnter(AIComponent& ai) {};
        virtual void OnExit(AIComponent& ai) {};
        virtual std::unique_ptr<EnemyState> Update(AIComponent& ai) = 0;
    };

    class PatrolState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override {}
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    };

    class ChaseState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override;
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    private:
        float m_LostSightTimer = 0.f;
        static constexpr float k_LostSightTimeout = 2.f;
    };

    class AttackState final : public EnemyState
    {
    public:
        void OnEnter(AIComponent& ai) override {}
        std::unique_ptr<EnemyState> Update(AIComponent& ai) override;
    };
}
#endif