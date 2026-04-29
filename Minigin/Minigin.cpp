#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"

#include <thread>

#include "AudioService.h"
#include "CollisionManager.h"
#include "EventQueue.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "ServiceLocator.h"

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
	ServiceLocator::GetPlatform().Init();

	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);

	#ifdef _DEBUG
	auto realAudio = std::make_unique<dae::AudioService>();
	auto loggedAudio = std::make_unique<dae::LoggingAudioService>(std::move(realAudio));
	ServiceLocator::RegisterAudioService(std::move(loggedAudio));
	#else
	auto audioService = std::make_unique<dae::AudioService>();
	EventQueue::GetInstance().GetNotifier()->AddObserver(audioService.get());
	ServiceLocator::RegisterAudioService(std::move(audioService));
	#endif

	dae::ServiceLocator::GetAudioService().Init();
}

dae::Minigin::~Minigin()
{
	ServiceLocator::GetPlatform().Shutdown();

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	ServiceLocator::RegisterAudioService(std::make_unique<NullAudioService>());
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();
#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();
	SceneManager::GetInstance().ClearScenes();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void dae::Minigin::RunOneFrame()
{
	const int ms_per_frame = 16;

	Time::GetInstance().Update();

	auto frame_start_time = Time::GetInstance().GetLastTime();

	m_quit = !InputManager::GetInstance().ProcessInput();
	SceneManager::GetInstance().Update();
	ServiceLocator::GetPlatform().Update();
	CollisionManager::GetInstance().Update();
	EventQueue::GetInstance().Process();
	Renderer::GetInstance().Render();


	auto sleep_time = frame_start_time + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();

	std::this_thread::sleep_for(sleep_time);
}
