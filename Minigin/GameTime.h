#ifndef GAME_TIME_H  
#define GAME_TIME_H
#include <chrono>

class Time  final
{
public:
	static Time& GetInstance()
	{
		if (!instance)
			instance = new Time();

		return *instance;
	}

	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

	float GetDeltaTime() const { return deltaTime; }
	void Update()
	{
		const auto current_time = std::chrono::high_resolution_clock::now();

		deltaTime = std::chrono::duration<float>(current_time - lastTime).count();

		lastTime = current_time;
	}

	std::chrono::steady_clock::time_point GetLastTime() const
	{
		return lastTime;
	}

private:

	Time() = default;
	~Time() = default;

	inline static Time* instance = nullptr;

	float deltaTime = 0 ;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

};

#endif