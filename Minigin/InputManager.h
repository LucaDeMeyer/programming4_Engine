#pragma once
#include <map>

#include "Command.h"
#include "Controller.h"
#include "Singleton.h"

namespace dae
{

	enum class InputState
	{
		Down,
		Pressed,
		Up
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();

		void BindControllerCommand(unsigned int controllerIndex, Controller::ControllerButton button, InputState state, std::unique_ptr<Command> command);
		void BindKeyCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command);

	private:
		using CommandKey = std::pair<SDL_Keycode, InputState>;
		std::map<CommandKey, std::unique_ptr<Command>> m_KeyboardCommands;

		using ControllerKey = std::tuple<unsigned int, Controller::ControllerButton, InputState>;
		std::map<ControllerKey, std::unique_ptr<Command>> m_ConsoleCommands;

		std::vector<std::unique_ptr<Controller>> m_Controllers;
	};

}
