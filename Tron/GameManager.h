#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <vector>
#include "Singleton.h"
#include "Observer.h"
namespace Tron
{
	enum class GameMode
	{
		singlePlayer,
		COOP,
		PVP
	};

	class GameManager : public dae::Singleton<GameManager>, public dae::Observer
	{
	public:
		void Init();

		void SetGameMode(GameMode mode) { m_CurrentMode = mode; }
		GameMode GetGameMode() { return m_CurrentMode; }
		void RegisterEntiy(dae::GameObject* entity);
		void OnNotify(dae::GameObject* pEntity, const dae::Event& event) override;
		void Update(){}

		void ClearEntities() { for (auto* enity : m_Entities) RemoveEntity(enity); }
	private:
		friend class dae::Singleton<GameManager>;
		GameManager() = default;

		GameMode m_CurrentMode{ GameMode::singlePlayer };
		std::vector<dae::GameObject*> m_Entities{}; 

		void RemoveEntity(dae::GameObject* entity);
		void CheckWinCondition();
	};
}
#endif	