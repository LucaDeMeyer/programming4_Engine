#include "AudioService.h"

#include "EventQueue.h"
#include "ServiceLocator.h"
#include "Utils.h"

#ifdef WIN32
#include <iostream>
#include <unordered_map>

#include "SDL3_mixer/SDL_mixer.h"
class dae::AudioService::AudioServiceImpl
{
	MIX_Mixer* m_Mixer = nullptr;
	std::unordered_map<unsigned int, MIX_Audio*> m_LoadedSounds;

	MIX_Track* m_AmbientTrack = nullptr;
	std::vector<MIX_Track*> m_SFXTracks;
	int m_CurrentTrackIndex = 0;

	std::unordered_map<MIX_Track*, unsigned int> m_TrackHistory;


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

		m_AmbientTrack = MIX_CreateTrack(m_Mixer);
		if (!m_AmbientTrack)
		{
			std::cerr << "Failed to create track: " << SDL_GetError() << '\n';
			return false;
		}

		for (int i = 0; i < 16; ++i)
		{
			MIX_Track* newTrack = MIX_CreateTrack(m_Mixer);
			m_SFXTracks.push_back(newTrack);
		}

		return true;

	}
	void play(unsigned int soundHash, float volume, AudioType type)
	{
		auto it = m_LoadedSounds.find(soundHash);
		if (it == m_LoadedSounds.end())
		{
			std::cerr << "Tried to play sound ID " << soundHash << " but it wasn't loaded!\n";
			return;
		}

		MIX_Track* targetTrack = nullptr;
		if (type == AudioType::Ambient)
		{
			targetTrack = m_AmbientTrack;
			MIX_SetTrackAudio(targetTrack, it->second);
			MIX_SetTrackGain(targetTrack, volume);
			SDL_PropertiesID props = SDL_CreateProperties();
			SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
			
			MIX_PlayTrack(targetTrack, props);
			m_TrackHistory[targetTrack] = soundHash;
		}
		else if (type == AudioType::FX)
		{
			
			targetTrack = m_SFXTracks[m_CurrentTrackIndex];
			m_CurrentTrackIndex = (m_CurrentTrackIndex + 1) % m_SFXTracks.size();

			MIX_SetTrackAudio(targetTrack, it->second);
			MIX_SetTrackGain(targetTrack, volume);
			MIX_PlayTrack(targetTrack, 0); 
			m_TrackHistory[targetTrack] = soundHash;
		}
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

	void Pause(unsigned int soundHash, AudioType )
	{
		for (auto& pair : m_TrackHistory)
		{
			if (pair.second == soundHash)
			{
				MIX_PauseTrack(pair.first);
			}
		}
	}

	void Stop(unsigned int soundHash, AudioType )
	{
		for (auto& pair : m_TrackHistory)
		{
			if (pair.second == soundHash)
			{
			
				MIX_StopTrack(pair.first,0);
				pair.second = 0;
			}
		}
	}

	void StopAll() 
	{
		MIX_StopAllTracks(m_Mixer,0);
		m_TrackHistory.clear();
	}

	~AudioServiceImpl()
	{
		
		for (auto& pair : m_LoadedSounds) {
			MIX_DestroyAudio(pair.second);
		}

		for (MIX_Track* track : m_SFXTracks) {
			if (track) MIX_DestroyTrack(track);
		}
		if (m_AmbientTrack) MIX_DestroyTrack(m_AmbientTrack);
		if (m_Mixer) MIX_DestroyMixer(m_Mixer);
		MIX_Quit(); // this is causing crashes 
	}

};
#endif

bool dae::AudioService::Init() {return  m_pImpl->Init(); }
void dae::AudioService::LoadSound(unsigned int soundHash, const std::string& filepath) { m_pImpl->LoadSound(soundHash, filepath); }
void dae::AudioService::Play(unsigned int soundHash, float volume, AudioType type)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_CommandQueue.push({ soundHash, volume,type,AudioCommandType::Play });
	m_Condition.notify_one();
}

void dae::AudioService::Pause(unsigned int soundHash, AudioType type)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	m_CommandQueue.push({  soundHash, 0.0f, type,AudioCommandType::Pause, });
	m_Condition.notify_one();
}

void dae::AudioService::Stop(unsigned int soundHash, AudioType type)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	m_CommandQueue.push({ soundHash, 0.0f, type,AudioCommandType::Stop, });
	m_Condition.notify_one();
}
void dae::AudioService::StopAll()
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_CommandQueue.push({  0, 0.0f, AudioType::FX,AudioCommandType::StopAll });
	m_Condition.notify_one();
}

void dae::AudioService::OnNotify(GameObject*, const Event& event)
{
	if (event.ID == Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"))
	{
		auto* soundArgs = static_cast<SoundARGS*>(event.pArgs.get());
		if (soundArgs) {
			Play(soundArgs->soundHash, soundArgs->volume,soundArgs->type);
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

		switch (command.commandType)
		{
		case AudioCommandType::Play:
			m_pImpl->play(command.soundHash, command.volume, command.type);
			break;
		case AudioCommandType::Pause:
			m_pImpl->Pause(command.soundHash,command.type);
			break;
		case AudioCommandType::Stop:
			m_pImpl->Stop(command.soundHash,command.type);
			break;
		case AudioCommandType::StopAll:
			m_pImpl->StopAll();
			break;
		}
	}
}


dae::AudioService::AudioService()
	: m_pImpl(std::make_unique<AudioServiceImpl>())
{
	m_IsPlaying = true;

	//EventQueue::GetInstance().GetNotifier()->AddObserver(this);
	m_AudioThread = std::jthread(&AudioService::AudioThreadLoop, this);
}

dae::AudioService::~AudioService()
{
	m_IsPlaying = false;
	m_Condition.notify_one();
}

dae::LoggingAudioService::LoggingAudioService(std::unique_ptr<IAudioService> wrappedService)
{
	m_AudioService = std::move(wrappedService);
	EventQueue::GetInstance().GetNotifier()->AddObserver(this);
}

bool dae::LoggingAudioService::Init()
{
	std::cout << "Initializing Audio Service\n";
	return m_AudioService->Init();
}

void dae::LoggingAudioService::LoadSound(unsigned int soundHash, const std::string& filepath)
{
	std::cout << "Loading Sound: " << filepath << "at Hash: " << soundHash << '\n';
	m_AudioService->LoadSound(soundHash, filepath);
}

void dae::LoggingAudioService::Play(unsigned int soundHash, float volume, AudioType type)
{
	std::cout << "Playing sound: " << soundHash << "at volume: " << volume << '\n';
	m_AudioService->Play(soundHash, volume,type);
}

void dae::LoggingAudioService::OnNotify(GameObject* , const Event& event)
{
	if (event.ID == Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"))
	{
		std::cout << "Firing Audio Event\n";
		auto* soundArgs = static_cast<SoundARGS*>(event.pArgs.get());
		if (soundArgs) {
			Play(soundArgs->soundHash, soundArgs->volume, soundArgs->type);
		}
	}
}

void dae::LoggingAudioService::Pause(unsigned int soundHash, AudioType type)
{
	std::cout << "Pausing Audio: " << soundHash << '\n';
	m_AudioService->Pause(soundHash, type);
}

void dae::LoggingAudioService::Stop(unsigned int soundHash, AudioType type)
{

	std::cout << "Stopping Audio: " << soundHash << '\n';
	m_AudioService->Stop(soundHash, type);
}
void dae::LoggingAudioService::StopAll() {
	std::cout << "Stopping all audio tracks.\n";
	m_AudioService->StopAll();
}