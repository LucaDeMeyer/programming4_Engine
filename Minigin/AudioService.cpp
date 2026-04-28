#include "AudioService.h"

#include "EventQueue.h"
#include "Utils.h"

#ifdef WIN32
#include <iostream>
#include <unordered_map>

#include "SDL3_mixer/SDL_mixer.h"
class dae::AudioService::AudioServiceImpl
{
	MIX_Mixer* m_Mixer = nullptr;
	std::unordered_map<unsigned int, MIX_Audio*> m_LoadedSounds;
	MIX_Track* m_EffectTrack = nullptr;

public:
	bool Init()
	{
		if (!MIX_Init())
		{
			std::cerr << "Failed to init SDL3_mixer: " << SDL_GetError() << '\n';
			return false;
		}

		m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (!m_Mixer)
		{
			std::cerr << "Error: " << SDL_GetError() << '\n';
			return false;
		}

		m_EffectTrack = MIX_CreateTrack(m_Mixer);
		if (!m_EffectTrack)
		{
			std::cerr << "Failed to create track: " << SDL_GetError() << '\n';
			return false;
		}
		return true;

	}
	void play(unsigned int soundHash, float volume)
	{
		auto it = m_LoadedSounds.find(soundHash);
		if (it == m_LoadedSounds.end())
		{
			std::cerr << "Tried to play sound ID " << soundHash << " but it wasn't loaded!\n";
			return;
		}
		MIX_SetTrackAudio(m_EffectTrack, it->second);

		MIX_SetTrackGain(m_EffectTrack, volume);

		MIX_PlayTrack(m_EffectTrack,0);
	}

	void LoadSound(unsigned int soundHash, const std::string& filepath)
	{
		if (m_LoadedSounds.contains(soundHash)) return;

		MIX_Audio* newSound = MIX_LoadAudio(m_Mixer, filepath.c_str(), true);
		if (newSound == nullptr)
		{
			std::cerr << "Failed to load sound: " << filepath << " Error: " << SDL_GetError() << '\n';
			return;
		}

		m_LoadedSounds[soundHash] = newSound;
	}

	~AudioServiceImpl()
	{
		
		for (auto& pair : m_LoadedSounds) {
			MIX_DestroyAudio(pair.second);
		}

		if (m_EffectTrack) MIX_DestroyTrack(m_EffectTrack);
		if (m_Mixer) MIX_DestroyMixer(m_Mixer);
		MIX_Quit();
	}

};
#endif

bool dae::AudioService::Init() {return  m_pImpl->Init(); }
void dae::AudioService::LoadSound(unsigned int soundHash, const std::string& filepath) { m_pImpl->LoadSound(soundHash, filepath); }
void dae::AudioService::Play(unsigned int soundHash, float volume)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_CommandQueue.push({ soundHash, volume });
	m_Condition.notify_one();
}

void dae::AudioService::OnNotify(GameObject*, const Event& event)
{
	if (event.ID == Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"))
	{
		auto* soundArgs = static_cast<SoundARGS*>(event.pArgs.get());
		if (soundArgs) {
			Play(soundArgs->soundHash, 1.0f);
		}
	}
}

void dae::AudioService::AudioThreadLoop()
{
	while (m_IsPlaying)
	{
		AudioCommand command;
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Condition.wait(lock, [this]() {
				return !m_CommandQueue.empty() || !m_IsPlaying;
				});

			if (!m_IsPlaying && m_CommandQueue.empty()) break;

			command = m_CommandQueue.front();
			m_CommandQueue.pop();
		} 
		m_pImpl->play(command.soundHash, command.volume);
	}
}


dae::AudioService::AudioService()
	: m_pImpl(std::make_unique<AudioServiceImpl>())
{
	m_IsPlaying = true;

	EventQueue::GetInstance().GetNotifier()->AddObserver(this);
	m_AudioThread = std::jthread(&AudioService::AudioThreadLoop, this);
}

dae::AudioService::~AudioService()
{
	m_IsPlaying = false;
	m_Condition.notify_one();
}