#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <vector>

#include "GameActorComponent.h"
#include "Singleton.h"
#include "Observer.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "glm/vec2.hpp"

namespace Tron
{
	enum class GameMode
	{
		singlePlayer,
		COOP,
		PVP
	};

	enum class TransitionType
	{
		None,
		NextLevel,
		HighScore,
		Menu,
		PvpWinner
	};

	struct HighScoreEntry
	{
		std::string name;
		int score;

		bool operator > (const HighScoreEntry& other) const
		{
			return score > other.score;
		}
	};

	class GameManager : public dae::Singleton<GameManager>, public dae::Observer
	{
	public:
		void Init();

		void SetGameMode(GameMode mode) { m_CurrentMode = mode; }
		GameMode GetGameMode() { return m_CurrentMode; }
		void RegisterEntiy(dae::GameObject* entity);
		void OnNotify(dae::GameObject* pEntity, const dae::Event& event) override;
		void Update();

		void ClearEntities()
		{

			m_Entities.clear();
			m_Players = 0;
			m_enemies = 0;
			dae::ServiceLocator::GetMemoryPoolService().ClearAll();
		}

		void AddScore(const std::string& name, int score);

		int m_P1Score{};
		int m_p2Score{};
		int m_P1Lives{};
		int m_P2Lives{};

		int m_LVLNR{};

		int GetTotalLevelsCleared() const { return m_LVLNR; }

		void SetTransitioning(bool state) { m_IsTransitioningLevel = state; }

		const std::vector<HighScoreEntry>& GetHighScores() const
		{
			return m_HighScores;
		}

		void ResetGameStats()
		{
			m_P1Score = 0;
			m_p2Score = 0;
			m_LVLNR = 0;
			
		}
		void SetWindowSize(glm::vec2 windowSize) { m_WindowSize = windowSize; }
		glm::vec2 GetWindowSize() const& { return m_WindowSize; }

		std::vector<dae::GameObject*> GetEntities() { return m_Entities; }

	private:
		friend class dae::Singleton<GameManager>;
		GameManager() = default;


		void RemoveEntity(dae::GameObject* entity);
		void CheckWinCondition();

		void SaveToFile();
		void LoadFile();

		GameMode m_CurrentMode{ GameMode::singlePlayer };
		std::vector<dae::GameObject*> m_Entities{}; 

		std::vector<HighScoreEntry> m_HighScores;
		const std::string m_FileName = "highscores.txt";

		int m_enemies{};
		int m_Players{};
	
		
		
		glm::vec2 m_WindowSize{};

		float m_TransitionTimer{ 0.0f };
		const float m_TransitionDelay{ 1.5f }; 
		TransitionType m_PendingTransition{ TransitionType::None };

		bool m_IsTransitioningLevel = false;
	};
}
#endif	