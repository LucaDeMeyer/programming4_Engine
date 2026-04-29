#ifndef TRON_EVENTS_H
#define TRON_EVENTS_H
#include "Event.h"

struct LivesChangedARGS : public dae::EventARGS
{
	int currentLives;
	LivesChangedARGS(int _currentLives) : currentLives(_currentLives){}
};

struct PlayerDiedARGS : public dae::EventARGS
{
	int PlayerID;
	PlayerDiedARGS(int _playerID) : PlayerID(_playerID){}
};

struct ScoreGainedARGS : public dae::EventARGS
{
	int points;
	explicit ScoreGainedARGS(int p) : points(p) {}
};

struct ActorDied : public dae::EventARGS
{
	dae::GameObject* obj;
	explicit ActorDied(dae::GameObject* _obj) : obj(_obj){}
};
struct Teleport : public dae::EventARGS
{
	dae::GameObject* obj;
	explicit Teleport(dae::GameObject* _obj) : obj(_obj){}
};
#endif