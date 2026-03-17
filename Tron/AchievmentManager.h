#ifndef ACHIEVEMENT_MANAGER_H
#define ACHIEVEMENT_MANAGER_H
#include "SteamManager.h"
#include "Singleton.h"
#include "Observer.h"
#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

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
		AchievementManager() = default;
		virtual ~AchievementManager() = default;

		int m_CurrentScore = 0;
		const int WINNER_THRESHOLD = 500;

		void CheckUnlockCondition();

		STEAM_CALLBACK(AchievementManager, OnUserStatsReceived, UserStatsReceived_t);
		STEAM_CALLBACK(AchievementManager, AchievementUnlocked, UserAchievementStored_t);
	};
}
#endif	