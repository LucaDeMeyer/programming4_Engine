#pragma once
#include <map>
#include <SDL3/SDL_keycode.h>
#include "Command.h"
#include "Controller.h"
#include "Singleton.h"
#include "glm/vec2.hpp"

namespace dae
{
    /// @brief Classifies when a bound command should fire relative to a button's state.
    enum class InputState
    {
        Down,    ///< Fire once on the frame the button is first pressed (rising edge).
        Pressed, ///< Fire every frame while the button is held.
        Up       ///< Fire once on the frame the button is released (falling edge).
    };

    /// @brief Singleton that routes hardware input to bound Command objects each frame.
    ///
    /// Supports four input categories:
    ///  - **Keyboard commands** — bound to a key + InputState pair via BindKeyCommand().
    ///  - **Controller commands** — bound to a button + InputState pair via BindControllerCommand().
    ///  - **Continuous commands** — executed unconditionally every frame via BindContinuousCommand().
    ///  - **Movement commands** — last-input-wins stack that ensures only the most recently
    ///    pressed direction key (or button) is active at any time.
    ///
    /// Call ProcessInput() once per frame; it polls SDL events and all connected controllers,
    /// then dispatches the appropriate commands. Use ClearAllCommands() when loading a new
    /// scene, or RemoveCommandsForObject() to unbind commands for a specific GameObject.
    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();

        /// @brief Polls SDL events and controller state, then dispatches bound commands.
        /// @return False if a quit event was received (SDL_EVENT_QUIT), true otherwise.
        bool ProcessInput();

        /// @brief Removes all commands whose ActorCommand target is @p object.
        ///
        /// Safe to call mid-frame; removals are deferred and applied at the start
        /// of the next ProcessInput() call.
        /// @param object The GameObject whose commands should be unbound.
        void RemoveCommandsForObject(GameObject* object);

        /// @brief Binds a command to a controller button and input state.
        /// @param controllerIndex Zero-based index of the target controller.
        /// @param button          The button to listen for.
        /// @param state           When to fire (Down, Pressed, or Up).
        /// @param command         Owning pointer to the command to execute.
        void BindControllerCommand(unsigned int controllerIndex,
            Controller::ControllerButton button,
            InputState state,
            std::unique_ptr<Command> command);

        /// @brief Binds a command to a keyboard key and input state.
        /// @param key     The SDL keycode to listen for.
        /// @param state   When to fire (Down, Pressed, or Up).
        /// @param command Owning pointer to the command to execute.
        void BindKeyCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command);

        /// @brief Registers a command that executes unconditionally every frame.
        /// @param command Owning pointer to the command to execute each tick.
        void BindContinuousCommand(std::unique_ptr<Command> command);

        /// @brief Returns the current mouse cursor position in window space.
        glm::vec2 GetMousePosition() const;

        /// @brief Returns the right thumbstick axis for the given controller, normalised to [-1, 1].
        /// @param controllerIndex Zero-based index of the target controller.
        glm::vec2 GetRightThumbstick(unsigned int controllerIndex) const;

        /// @brief Registers a keyboard movement command using last-input-wins priority.
        ///
        /// When multiple movement keys are held simultaneously, only the command for the
        /// most recently pressed key executes. Releasing that key automatically reactivates
        /// the previous one (stack-based).
        /// @param key     The SDL keycode that triggers this movement.
        /// @param command Owning pointer to the directional command.
        void RegisterMovementCommand(SDL_Keycode key, std::unique_ptr<Command> command);

        /// @brief Registers a controller movement command using last-input-wins priority.
        ///
        /// Mirrors RegisterMovementCommand() for controller buttons; each controller index
        /// maintains its own independent direction stack.
        /// @param controllerIndex Zero-based index of the target controller.
        /// @param button          The directional button to listen for.
        /// @param command         Owning pointer to the directional command.
        void RegisterControllerMovementCommand(unsigned int controllerIndex,
            Controller::ControllerButton button,
            std::unique_ptr<Command> command);

        /// @brief Removes all bound commands of every category and clears controller state.
        void ClearAllCommands();

        /// @brief Returns the number of currently connected controllers.
        int GetConnectedControllerCount() const;

    private:
        /// @brief Processes deferred object-command removals queued by RemoveCommandsForObject().
        void RemoveCommands();

        /// @brief Checks whether the mouse cursor overlaps any clickable UI elements this frame.
        /// @param mousePos Current cursor position in window space.
        void HandleButtonClick(const glm::vec2& mousePos);

        // --- Keyboard bindings ---
        using CommandKey = std::pair<SDL_Keycode, InputState>;
        std::map<CommandKey, std::unique_ptr<Command>> m_KeyboardCommands;

        // --- Controller bindings ---
        using ControllerKey = std::tuple<unsigned int, Controller::ControllerButton, InputState>;
        std::map<ControllerKey, std::unique_ptr<Command>> m_ConsoleCommands;

        // --- Per-frame continuous commands ---
        std::vector<std::unique_ptr<Command>> m_ContinuousCommands;

        // --- Connected controllers ---
        std::vector<std::unique_ptr<Controller>> m_Controllers;

        // --- Deferred removal queue ---
        std::vector<GameObject*> m_ObjectsToClear;

        // --- Keyboard movement stack ---
        struct MovementBinding {
            SDL_Keycode key;
            std::unique_ptr<Command> command;
        };
        std::vector<MovementBinding> m_MovementBindings; ///< All registered keyboard movement bindings.
        std::vector<SDL_Keycode>     m_MovementKeyStack; ///< Keys in press order; top = active direction.

        // --- Controller movement stack ---
        struct ControllerMovementBinding {
            unsigned int controllerIndex;
            Controller::ControllerButton button;
            std::unique_ptr<Command> command;
        };

        using ControllerButtonID = std::pair<unsigned int, Controller::ControllerButton>;
        std::vector<ControllerMovementBinding> m_ControllerMovementBindings;
        std::vector<ControllerButtonID> m_ControllerMovementStacks[2]; ///< One stack per supported controller slot.
    };

} // namespace dae