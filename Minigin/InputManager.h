#pragma once
#include <map>
#include <SDL3/SDL_keycode.h>
#include "Command.h"
#include "Controller.h"
#include "Singleton.h"
#include "glm/vec2.hpp"

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
		void RemoveCommandsForObject(GameObject* object);

		void BindControllerCommand(unsigned int controllerIndex, Controller::ControllerButton button, InputState state, std::unique_ptr<Command> command);
		void BindKeyCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command);

		void BindContinuousCommand(std::unique_ptr<Command> command);
		glm::vec2 GetMousePosition() const;
		glm::vec2 GetRightThumbstick(unsigned int controllerIndex) const;

		void RegisterMovementCommand(SDL_Keycode key, std::unique_ptr<Command> command);

		void RegisterControllerMovementCommand(unsigned int controllerIndex,
			Controller::ControllerButton button,
			std::unique_ptr<Command> command);

		void ClearAllCommands();

	private:

		void RemoveCommands();
		void HandleButtonClick(const glm::vec2& mousePos);
	
		using CommandKey = std::pair<SDL_Keycode, InputState>;
		std::map<CommandKey, std::unique_ptr<Command>> m_KeyboardCommands;

		using ControllerKey = std::tuple<unsigned int, Controller::ControllerButton, InputState>;
		std::map<ControllerKey, std::unique_ptr<Command>> m_ConsoleCommands;

		std::vector<std::unique_ptr<Command>> m_ContinuousCommands;

		std::vector<std::unique_ptr<Controller>> m_Controllers;

		std::vector<GameObject*> m_ObjectsToClear;

		struct MovementBinding {
			SDL_Keycode key;
			std::unique_ptr<Command> command;
		};
		std::vector<MovementBinding> m_MovementBindings;   
		std::vector<SDL_Keycode>     m_MovementKeyStack;

		struct ControllerMovementBinding {
			unsigned int controllerIndex;
			Controller::ControllerButton button;
			std::unique_ptr<Command> command;
		};

		using ControllerButtonID = std::pair<unsigned int, Controller::ControllerButton>;
		std::vector<ControllerMovementBinding> m_ControllerMovementBindings;
		std::vector<ControllerButtonID>        m_ControllerMovementStack;

	};

}
