#ifndef TRON_EVENTTS_H
#define TRON_EVENTS_H
#include "Observer.h"

struct AddScore : public dae::Event
{
	AddScore(int _score) : Score(_score){}
	int Score;
};

struct PlayerDied : public dae::Event
{
	PlayerDied(int _playerID) : PlayerID(_playerID){} 
	int PlayerID;
};
struct LivesChanged : public dae::Event
{
	LivesChanged(int lives) : currentLives(lives){}
	int currentLives;
};
#endif
