#include "Controller.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <XInput.h>

#pragma comment(lib, "xinput.lib")

using namespace dae;
class Controller::ControllerImpl
{
public:
    ControllerImpl(unsigned int controllerIndex) : m_ControllerIndex(controllerIndex)
    {
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
    }

    void Update()
    {
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_ControllerIndex, &m_CurrentState);

        auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
        m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
    }

    bool IsDownThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
    bool IsUpThisFrame(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
    bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; }

    glm::vec2 GetRightThumbstick() const
    {
        float x = m_CurrentState.Gamepad.sThumbRX / 32767.0f;
        float y = m_CurrentState.Gamepad.sThumbRY / 32767.0f;

        // Clamp in case of exactly -32768
        if (x < -1.0f) x = -1.0f;
        if (y < -1.0f) y = -1.0f;

        return { x, -y };
    }

private:
    unsigned int m_ControllerIndex;
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};

    WORD m_ButtonsPressedThisFrame{};
    WORD m_ButtonsReleasedThisFrame{};
};
#else

#include <SDL3/SDL.h>
#include <cstdint>

using namespace dae; 
class Controller::ControllerImpl
{
public:
    ControllerImpl(unsigned int controllerIndex) : m_ControllerIndex(controllerIndex)
    {
    }
    ~ControllerImpl()
    {
        if (m_Gamepad) {
            SDL_CloseGamepad(m_Gamepad);
        }
    }
    void Update()
    {

        if (!m_Gamepad) {
            int count = 0;
            SDL_JoystickID* gamepads = SDL_GetGamepads(&count);

            if (gamepads && static_cast<int>(m_ControllerIndex) < count) {
                m_Gamepad = SDL_OpenGamepad(gamepads[m_ControllerIndex]);
            }
            if (gamepads) SDL_free(gamepads);
        }

        m_PreviousButtons = m_CurrentButtons;
        m_CurrentButtons = 0;

        if (!m_Gamepad) return;

        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP)) m_CurrentButtons |= 0x0001;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) m_CurrentButtons |= 0x0002;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) m_CurrentButtons |= 0x0004;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) m_CurrentButtons |= 0x0008;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_START)) m_CurrentButtons |= 0x0010;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_BACK)) m_CurrentButtons |= 0x0020;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK)) m_CurrentButtons |= 0x0040;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK)) m_CurrentButtons |= 0x0080;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER)) m_CurrentButtons |= 0x0100;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)) m_CurrentButtons |= 0x0200;
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_SOUTH)) m_CurrentButtons |= 0x1000; // A Button
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_EAST)) m_CurrentButtons |= 0x2000; // B Button
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_WEST)) m_CurrentButtons |= 0x4000; // X Button
        if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_NORTH)) m_CurrentButtons |= 0x8000; // Y Button

        auto buttonChanges = m_CurrentButtons ^ m_PreviousButtons;
        m_ButtonsPressedThisFrame = buttonChanges & m_CurrentButtons;
        m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentButtons);
    }
    bool IsDownThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; }
    bool IsUpThisFrame(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; }
    bool IsPressed(unsigned int button) const { return m_CurrentButtons & button; }
    glm::vec2 GetRightThumbstick() const
    {
        if (!m_Gamepad) return { 0.0f, 0.0f };

        // Normalize from [-32768, 32767] to [-1.0f, 1.0f]
        float x = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / 32767.0f;
        float y = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / 32767.0f;

        if (x < -1.0f) x = -1.0f;
        if (y < -1.0f) y = -1.0f;

        // SDL is already positive-DOWN, so we do NOT invert it!
        return { x, y };
    }
private:
    SDL_Gamepad* m_Gamepad = nullptr;
    unsigned int m_ControllerIndex;
    uint16_t m_PreviousButtons = 0;
    uint16_t m_CurrentButtons = 0;
    uint16_t m_ButtonsPressedThisFrame = 0;
    uint16_t m_ButtonsReleasedThisFrame = 0;
};
#endif
Controller::Controller(unsigned int controllerIndex)
    : m_pImpl(std::make_unique<ControllerImpl>(controllerIndex)) {
}

Controller::~Controller() = default;

void Controller::Update() { m_pImpl->Update(); }

bool Controller::IsDownThisFrame(ControllerButton button) const { return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button)); }
bool Controller::IsUpThisFrame(ControllerButton button) const { return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button)); }
bool Controller::IsPressed(ControllerButton button) const { return m_pImpl->IsPressed(static_cast<unsigned int>(button)); }
glm::vec2 Controller::GetRightThumbstick() const { return m_pImpl->GetRightThumbstick(); }