#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <vector>

#include "GameActorComponent.h"
#include "Singleton.h"
#include "Observer.h"
#include "GameObject.h"
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

		void ClearEntities()
		{
			for (auto* entity : m_Entities)
			{
				if (!entity) continue;
				auto* actor = entity->GetComponent<GameActor>();
				if (actor)
					actor->GetEventSubject().RemoveObserver(this);
			}


			m_Entities.clear();
			m_Players = 0;
			m_enemies = 0;
		}
	private:
		friend class dae::Singleton<GameManager>;
		GameManager() = default;

		GameMode m_CurrentMode{ GameMode::singlePlayer };
		std::vector<dae::GameObject*> m_Entities{}; 

		void RemoveEntity(dae::GameObject* entity);
		void CheckWinCondition();

		int m_enemies{};
		int m_Players{};
	};
}
#endif	