#ifndef ACHIEVEMENT_MANAGER_H
#define ACHIEVEMENT_MANAGER_H
#include "Singleton.h"
#include "Observer.h"
namespace Tron
{
	class AchievementManager : public dae::Singleton<AchievementManager>, public dae::Observer
	{
	public:

		void Init();
		void AddScore(int amount);
		void OnNotify(dae::GameObject* obj, const dae::Event& event) override;

	private:
		friend class dae::Singleton<AchievementManager>;
		AchievementManager();
		~AchievementManager();
		class AchievementManagerImpl;
		std::unique_ptr<AchievementManagerImpl>m_Pimpl;

		int m_CurrentScore = 0;
		const int WINNER_THRESHOLD = 500;

		void CheckUnlockCondition();
	};
}
#endif	