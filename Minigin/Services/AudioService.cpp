#include "AudioService.h"
#include "EventQueue.h"
#include "ServiceLocator.h"
#include "Utils.h"

#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "SDL3_mixer/SDL_mixer.h"

class dae::AudioService::AudioServiceImpl
{
    MIX_Mixer* m_Mixer = nullptr;
    std::unordered_map<unsigned int, MIX_Audio*> m_LoadedSounds;
    MIX_Track* m_AmbientTrack = nullptr;
    std::vector<MIX_Track*> m_SFXTracks;
    int m_CurrentTrackIndex = 0;
    std::unordered_map<MIX_Track*, unsigned int> m_TrackHistory;

    enum class AudioCommandType { Play, Pause, Stop, StopAll, ToggleMute};

    struct AudioCommand
    {
        unsigned int soundHash;
        float volume;
        AudioType type;
        AudioCommandType commandType;
    };

    std::jthread m_AudioThread;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::atomic<bool> m_IsRunning{ false };
    bool m_IsMuted{ false };
    std::queue<AudioCommand> m_CommandQueue;

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
            m_SFXTracks.push_back(MIX_CreateTrack(m_Mixer));

        m_IsRunning = true;
        m_AudioThread = std::jthread(&AudioServiceImpl::ThreadLoop, this);

        return true;
    }

    void EnqueuePlay(unsigned int soundHash, float volume, AudioType type)
    {
        Enqueue({ soundHash, volume, type, AudioCommandType::Play });
    }

    void EnqueuePause(unsigned int soundHash, AudioType type)
    {
        Enqueue({ soundHash, 0.f, type, AudioCommandType::Pause });
    }

    void EnqueueStop(unsigned int soundHash, AudioType type)
    {
        Enqueue({ soundHash, 0.f, type, AudioCommandType::Stop });
    }

    void EnqueueStopAll()
    {
        Enqueue({ 0, 0.f, AudioType::FX, AudioCommandType::StopAll });
    }

    void EnqueueToggleMute()
    {
        Enqueue({ 0, 0.f, AudioType::FX, AudioCommandType::ToggleMute });
    }

    void LoadSound(unsigned int soundHash, const std::string& filepath)
    {
        if (m_LoadedSounds.contains(soundHash)) return;

        MIX_Audio* newSound = MIX_LoadAudio(m_Mixer, filepath.c_str(), true);
        if (!newSound)
        {
            std::cerr << "Failed to load sound: " << filepath << " Error: " << SDL_GetError() << '\n';
            return;
        }

        m_LoadedSounds[soundHash] = newSound;
    }

    ~AudioServiceImpl()
    {
       
        {
            std::lock_guard lock(m_Mutex);
            m_IsRunning = false;
        }
        m_Condition.notify_one();
        

        for (auto& [hash, audio] : m_LoadedSounds)
            MIX_DestroyAudio(audio);

        for (MIX_Track* track : m_SFXTracks)
            if (track) MIX_DestroyTrack(track);

        if (m_AmbientTrack) MIX_DestroyTrack(m_AmbientTrack);
        if (m_Mixer) MIX_DestroyMixer(m_Mixer);

        MIX_Quit();
    }

private:
    void Enqueue(AudioCommand cmd)
    {
        std::lock_guard lock(m_Mutex);
        m_CommandQueue.push(cmd);
        m_Condition.notify_one();
    }

    void ThreadLoop()
    {
        while (true)
        {
            AudioCommand command;
            {
                std::unique_lock lock(m_Mutex);
                m_Condition.wait(lock, [this]
                    {
                        return !m_CommandQueue.empty() || !m_IsRunning;
                    });

                if (!m_IsRunning && m_CommandQueue.empty()) break;

                command = m_CommandQueue.front();
                m_CommandQueue.pop();
            }

            switch (command.commandType)
            {
            case AudioCommandType::Play:    Play(command.soundHash, command.volume, command.type); break;
            case AudioCommandType::Pause:   Pause(command.soundHash); break;
            case AudioCommandType::Stop:    Stop(command.soundHash); break;
            case AudioCommandType::StopAll: StopAll(); break;
            case AudioCommandType::ToggleMute:  ToggleMuteInternal(); break;
            }
        }
    }

    void Play(unsigned int soundHash, float volume, AudioType type)
    {
        if (m_IsMuted) return;

        auto it = m_LoadedSounds.find(soundHash);
        if (it == m_LoadedSounds.end())
        {
            std::cerr << "Tried to play sound ID " << soundHash << " but it wasn't loaded!\n";
            return;
        }

        if (type == AudioType::Ambient)
        {
            MIX_SetTrackAudio(m_AmbientTrack, it->second);
            MIX_SetTrackGain(m_AmbientTrack, volume);
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
            MIX_PlayTrack(m_AmbientTrack, props);
            m_TrackHistory[m_AmbientTrack] = soundHash;
        }
        else if (type == AudioType::FX)
        {
            MIX_Track* track = m_SFXTracks[m_CurrentTrackIndex];
            m_CurrentTrackIndex = (m_CurrentTrackIndex + 1) % static_cast<int>(m_SFXTracks.size());
            MIX_SetTrackAudio(track, it->second);
            MIX_SetTrackGain(track, volume);
            MIX_PlayTrack(track, 0);
            m_TrackHistory[track] = soundHash;
        }
    }

    void Pause(unsigned int soundHash)
    {
        for (auto& [track, hash] : m_TrackHistory)
            if (hash == soundHash)
                MIX_PauseTrack(track);
    }

    void Stop(unsigned int soundHash)
    {
        for (auto& [track, hash] : m_TrackHistory)
            if (hash == soundHash)
            {
                MIX_StopTrack(track, 0);
                hash = 0;
            }
    }

    void StopAll()
    {
        MIX_StopAllTracks(m_Mixer, 0);
        m_TrackHistory.clear();
    }

    void ToggleMuteInternal()
    {
        m_IsMuted = !m_IsMuted;
        if (m_IsMuted)
        {
            MIX_PauseTrack(m_AmbientTrack); 
            for (auto* track : m_SFXTracks)
            {
                MIX_StopTrack(track, 0); 
            }
        }
        else
        {
            MIX_ResumeTrack(m_AmbientTrack); 
        }
    }
};


dae::AudioService::AudioService()
    : m_pImpl(std::make_unique<AudioServiceImpl>())
{
}

dae::AudioService::~AudioService() = default;

bool dae::AudioService::Init() { return m_pImpl->Init(); }
void dae::AudioService::LoadSound(unsigned int h, const std::string& path) { m_pImpl->LoadSound(h, path); }
void dae::AudioService::Play(unsigned int h, float v, AudioType t) { m_pImpl->EnqueuePlay(h, v, t); }
void dae::AudioService::Pause(unsigned int h, AudioType t) { m_pImpl->EnqueuePause(h, t); }
void dae::AudioService::Stop(unsigned int h, AudioType t) { m_pImpl->EnqueueStop(h, t); }
void dae::AudioService::StopAll() { m_pImpl->EnqueueStopAll(); }

void dae::AudioService::OnNotify(GameObject*, const Event& event)
{
    if (event.ID == Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"))
    {
        if (!event.pArgs) return;
        auto* args = static_cast<SoundARGS*>(event.pArgs);
        if (args) Play(args->soundHash, args->volume, args->type);
    }
}


dae::LoggingAudioService::LoggingAudioService(std::unique_ptr<IAudioService> wrappedService)
    : m_AudioService(std::move(wrappedService))
{
    EventQueue::GetInstance().GetNotifier()->AddObserver(this);
}

bool dae::LoggingAudioService::Init()
{
    std::cout << "Initializing Audio Service\n";
    return m_AudioService->Init();
}

void dae::LoggingAudioService::LoadSound(unsigned int h, const std::string& path)
{
    std::cout << "Loading sound: " << path << " at hash: " << h << '\n';
    m_AudioService->LoadSound(h, path);
}

void dae::LoggingAudioService::Play(unsigned int h, float v, AudioType t)
{
    std::cout << "Playing sound: " << h << " at volume: " << v << '\n';
    m_AudioService->Play(h, v, t);
}

void dae::LoggingAudioService::Pause(unsigned int h, AudioType t)
{
    std::cout << "Pausing audio: " << h << '\n';
    m_AudioService->Pause(h, t);
}

void dae::LoggingAudioService::Stop(unsigned int h, AudioType t)
{
    std::cout << "Stopping audio: " << h << '\n';
    m_AudioService->Stop(h, t);
}

void dae::LoggingAudioService::StopAll()
{
    std::cout << "Stopping all audio tracks.\n";
    m_AudioService->StopAll();
}

void dae::AudioService::ToggleMute() { m_pImpl->EnqueueToggleMute(); }

void dae::LoggingAudioService::ToggleMute()
{
    std::cout << "Toggling Mute State\n";
    m_AudioService->ToggleMute();
}

void dae::LoggingAudioService::OnNotify(GameObject*, const Event& event)
{
    if (event.ID == Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"))
    {
        std::cout << "Firing audio event\n";
        auto* args = static_cast<SoundARGS*>(event.pArgs);
        if (args) Play(args->soundHash, args->volume, args->type);
    }
}