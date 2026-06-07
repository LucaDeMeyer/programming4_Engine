#ifndef SDL_AUDIO
#define SDL_AUDIO
#include "Services.h"
#include "Observer.h"
#include <thread>
#include <mutex>
#include <queue>

namespace dae
{

	class AudioService : public IAudioService, public Observer
	{
	public:
		AudioService();
		~AudioService() override;
		bool Init() override;
		void Play(unsigned int soundHash, float volume, AudioType type) override;
		void Pause(unsigned int soundHash, AudioType type) override;
		void Stop(unsigned int soundHash, AudioType type) override;
		void StopAll() override;
		void ToggleMute() override;

		void LoadSound(unsigned int soundHash, const std::string& filepath) override;
		void OnNotify(GameObject* obj, const Event& event) override;

	private:
		class AudioServiceImpl;
		std::unique_ptr<AudioServiceImpl> m_pImpl;
	};

	class LoggingAudioService final :public IAudioService, public Observer
	{
	public:
		LoggingAudioService(std::unique_ptr<IAudioService> wrappedService);
		~LoggingAudioService()override = default;

		bool Init() override;
		void Play(unsigned int soundHash, float volume, AudioType type) override;
		void Pause(unsigned int soundHash, AudioType type) override;
		void Stop(unsigned int soundHash, AudioType type) override;
		void StopAll() override;
		void ToggleMute() override;

		void LoadSound(unsigned int soundHash, const std::string& filepath) override;

		void OnNotify(GameObject* obj, const Event& event) override;

	private:
		std::unique_ptr<IAudioService> m_AudioService;
	};
}
#endif
