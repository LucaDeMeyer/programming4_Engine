#ifndef TRON_EVENTTS_H
#define TRON_EVENTS_H
#include "Observer.h"
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

#endif