#include <SDL3/SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl3.h>


dae::InputManager::InputManager()
{
	for (unsigned int i = 0; i < 1; ++i) {
		m_Controllers.push_back(std::make_unique<Controller>(i));
	}
}

bool dae::InputManager::ProcessInput()
{
    for (auto& controller : m_Controllers) {
        if (controller) controller->Update();
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL3_ProcessEvent(&e);

        if (e.type == SDL_EVENT_QUIT) {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if (e.key.repeat == 0) {
                CommandKey key = { e.key.key, InputState::Down };
                if (m_KeyboardCommands.contains(key)) {
                    m_KeyboardCommands[key]->Execute();
                }
            }
        }
        else if (e.type == SDL_EVENT_KEY_UP) {
            CommandKey key = { e.key.key, InputState::Up };
            if (m_KeyboardCommands.contains(key)) {
                m_KeyboardCommands[key]->Execute();
            }
        }
    }
    int numKeys;
    const bool* pKeyboardState = SDL_GetKeyboardState(&numKeys);

    for (const auto& [keyPair, command] : m_KeyboardCommands) {
        if (keyPair.second == InputState::Pressed) {
            SDL_Scancode scancode = SDL_GetScancodeFromKey(keyPair.first, nullptr);
            if (scancode < numKeys && pKeyboardState[scancode]) {
                command->Execute();
            }
        }
    }

    for (const auto& [keyTuple, command] : m_ConsoleCommands) {
        unsigned int controllerIndex = std::get<0>(keyTuple);
        Controller::ControllerButton button = std::get<1>(keyTuple);
        InputState state = std::get<2>(keyTuple);
        if (controllerIndex < m_Controllers.size() && m_Controllers[controllerIndex]) {
            const auto& controller = m_Controllers[controllerIndex];
            bool shouldExecute = false;
            switch (state) {
            case InputState::Down:    shouldExecute = controller->IsDownThisFrame(button); break;
            case InputState::Up:      shouldExecute = controller->IsUpThisFrame(button); break;
            case InputState::Pressed: shouldExecute = controller->IsPressed(button); break;
            }

            if (shouldExecute) {
                command->Execute();
            }
        }
    }

    for (const auto& command : m_ContinuousCommands) {
        command->Execute();
    }

    RemoveCommands();

    return true;
}


void dae::InputManager::BindContinuousCommand(std::unique_ptr<Command> command)
{
    m_ContinuousCommands.push_back(std::move(command));
}

glm::vec2 dae::InputManager::GetMousePosition() const
{
    float x, y;
    SDL_GetMouseState(&x, &y);
    return { x, y };
}

glm::vec2 dae::InputManager::GetRightThumbstick(unsigned int controllerIndex) const
{
    if (controllerIndex < m_Controllers.size() && m_Controllers[controllerIndex]) {
        return m_Controllers[controllerIndex]->GetRightThumbstick();
    }
    return { 0.0f, 0.0f };
}


void dae::InputManager::BindKeyCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command)
{
	CommandKey mapKey = { key, state };
	m_KeyboardCommands[mapKey] = std::move(command);
}

void dae::InputManager::BindControllerCommand(unsigned int controllerIndex, Controller::ControllerButton button, InputState state, std::unique_ptr<Command> command)
{
	ControllerKey mapKey = { controllerIndex, button, state };
	m_ConsoleCommands[mapKey] = std::move(command);
}

void dae::InputManager::RemoveCommandsForObject(GameObject* object)
{

    if (object)
    {
        m_ObjectsToClear.push_back(object);
    }
}

void dae::InputManager::RemoveCommands()
{
    if (!m_ObjectsToClear.empty())
    {
        for (auto* deadObject : m_ObjectsToClear)
        {
            std::erase_if(m_KeyboardCommands, [deadObject](const auto& pair) {
                if (auto* actorCmd = dynamic_cast<dae::ActorCommand*>(pair.second.get())) {
                    return actorCmd->GetGameObject() == deadObject;
                }
                return false;
                });

            std::erase_if(m_ConsoleCommands, [deadObject](const auto& pair) {
                if (auto* actorCmd = dynamic_cast<dae::ActorCommand*>(pair.second.get())) {
                    return actorCmd->GetGameObject() == deadObject;
                }
                return false;
                });

            std::erase_if(m_ContinuousCommands, [deadObject](const auto& cmd) {
                if (auto* actorCmd = dynamic_cast<dae::ActorCommand*>(cmd.get())) {
                    return actorCmd->GetGameObject() == deadObject;
                }
                return false;
                });
        }

        m_ObjectsToClear.clear();
    }
}