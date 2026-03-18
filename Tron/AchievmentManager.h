#ifndef ACHIEVEMENT_MANAGER_H
#define ACHIEVEMENT_MANAGER_H

#include "Singleton.h"
#include "Observer.h"

namespace Tron
{
    class AchievementManager : public dae::Singleton<AchievementManager>, public dae::Observer
    {
    public:
        void OnNotify(dae::GameObject* obj, const dae::Event& event) override;
    private:
        friend class dae::Singleton<AchievementManager>;
        AchievementManager() = default;
    	~AchievementManager() override = default;

        int m_CurrentScore{ 0 };
        const int WINNER_THRESHOLD{ 500 };
        void CheckUnlockCondition();
    };
}
#endif