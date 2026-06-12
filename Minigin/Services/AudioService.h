#ifndef SDL_AUDIO
#define SDL_AUDIO
#include "Services.h"
#include "Observer.h"
#include <thread>
#include <mutex>
#include <queue>

namespace dae
{
	/// @brief Concrete audio service backed by SDL (or similar), with Observer support.
	///
	/// Implements IAudioService for real audio playback and also acts as an Observer
	/// so it can react to SoundARGS events fired through the EventQueue. Heavy
	/// platform work is delegated to the inner AudioServiceImpl (Pimpl) to keep
	/// this header lean and the SDL dependency out of consumer translation units.
	///
	/// Register with the EventQueue notifier at startup to enable event-driven playback:
	/// @code
	///   EventQueue::GetInstance().GetNotifier()->AddObserver(audioServicePtr);
	/// @endcode
	class AudioService : public IAudioService, public Observer
	{
	public:
		AudioService();
		~AudioService() override;

		/// @brief Initialises the SDL audio backend and worker thread.
		/// @return True on success, false if SDL audio could not be opened.
		bool Init() override;

		/// @brief Immediately queues a sound for playback on the worker thread.
		/// @param soundHash Hashed identifier of the sound to play.
		/// @param volume    Playback volume in [0.0, 1.0].
		/// @param type      Audio category (Ambient or FX).
		void Play(unsigned int soundHash, float volume, AudioType type) override;

		/// @brief Pauses a currently playing sound.
		/// @param soundHash Hashed identifier of the sound to pause.
		/// @param type      Audio category of the sound.
		void Pause(unsigned int soundHash, AudioType type) override;

		/// @brief Stops a currently playing sound.
		/// @param soundHash Hashed identifier of the sound to stop.
		/// @param type      Audio category of the sound.
		void Stop(unsigned int soundHash, AudioType type) override;

		/// @brief Stops all currently playing sounds.
		void StopAll() override;

		/// @brief Toggles global mute on/off.
		void ToggleMute() override;

		/// @brief Loads a sound asset from disk and maps it to @p soundHash.
		/// @param soundHash Hashed identifier to associate with the loaded asset.
		/// @param filepath  Path to the audio file on disk.
		void LoadSound(unsigned int soundHash, const std::string& filepath) override;

		/// @brief Observer callback — handles SoundARGS events from the EventQueue.
		/// @param obj   Unused; present for Observer interface compatibility.
		/// @param event Expected to carry a SoundARGS payload when relevant.
		void OnNotify(GameObject* obj, const Event& event) override;

	private:
		class AudioServiceImpl;
		std::unique_ptr<AudioServiceImpl> m_pImpl; ///< Platform-specific implementation (Pimpl).
	};

	/// @brief Decorator that adds console logging around any IAudioService implementation.
	///
	/// Wraps a concrete IAudioService and forwards all calls to it while printing
	/// a log message for each operation. Useful for debugging audio event flow
	/// without modifying the real audio service.
	///
	/// Also implements Observer so it can be registered with the EventQueue in
	/// place of (or alongside) the real AudioService.
	class LoggingAudioService final : public IAudioService, public Observer
	{
	public:
		/// @brief Constructs the logging decorator around an existing audio service.
		/// @param wrappedService The real audio service whose calls will be logged.
		LoggingAudioService(std::unique_ptr<IAudioService> wrappedService);
		~LoggingAudioService() override = default;

		/// @brief Initialises the wrapped service and logs the call.
		bool Init() override;

		/// @brief Logs the call then forwards to the wrapped service.
		void Play(unsigned int soundHash, float volume, AudioType type) override;

		/// @brief Logs the call then forwards to the wrapped service.
		void Pause(unsigned int soundHash, AudioType type) override;

		/// @brief Logs the call then forwards to the wrapped service.
		void Stop(unsigned int soundHash, AudioType type) override;

		/// @brief Logs the call then forwards to the wrapped service.
		void StopAll() override;

		/// @brief Logs the call then forwards to the wrapped service.
		void ToggleMute() override;

		/// @brief Logs the call then forwards to the wrapped service.
		void LoadSound(unsigned int soundHash, const std::string& filepath) override;

		/// @brief Observer callback — logs the event then forwards to the wrapped service's OnNotify.
		/// @param obj   Passed through unchanged.
		/// @param event Passed through unchanged.
		void OnNotify(GameObject* obj, const Event& event) override;

	private:
		std::unique_ptr<IAudioService> m_AudioService; ///< The decorated audio service.
	};

} // namespace dae
#endif