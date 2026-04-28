#ifndef SDL_AUDIO
#define SDL_AUDIO
#include "Services.h"
#include "Observer.h"
#include <thread>
#include <mutex>
#include <queue>

namespace dae
{
	enum class AudioType
	{
		
	};
	
	class AudioService : public IAudioService , public Observer
	{
	public:
		AudioService();
		~AudioService() override;

		bool Init() override;
		void Play(unsigned int soundHash, float volume) override;
		void LoadSound(unsigned int soundHash, const std::string& filepath);

		void OnNotify(GameObject* obj, const Event& event) override;

	private:
		struct AudioCommand {
			unsigned int soundHash;
			float volume;
		};

		class AudioServiceImpl;
		std::unique_ptr<AudioServiceImpl> m_pImpl;

		std::jthread m_AudioThread;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
		bool m_IsPlaying = false;
		std::queue<AudioCommand> m_CommandQueue;

		void AudioThreadLoop();
	};
}
#endif
