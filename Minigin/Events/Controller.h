#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>
#include "glm/vec2.hpp"

namespace dae
{
    /// @brief Wraps a single physical gamepad and exposes per-frame button state queries.
    ///
    /// Each Controller instance maps to one controller index (0-based). Call Update()
    /// once per frame to snapshot the current hardware state, then use the query
    /// methods to drive input handling. Uses a Pimpl to hide the platform-specific
    /// (XInput) implementation details.
    class Controller final
    {
    public:
        /// @brief Enumeration of all supported controller buttons.
        ///
        /// Values match the XInput bitmask constants so they can be combined
        /// or compared directly against raw hardware state if needed.
        enum class ControllerButton
        {
            DPadUp = 0x0001,
            DPadDown = 0x0002,
            DPadLeft = 0x0004,
            DPadRight = 0x0008,
            Start = 0x0010,
            Back = 0x0020,
            LeftThumb = 0x0040,
            RightThumb = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,
            ButtonA = 0x1000,
            ButtonB = 0x2000,
            ButtonX = 0x4000,
            ButtonY = 0x8000
        };

        /// @brief Constructs a Controller for the given hardware index.
        /// @param controllerIndex Zero-based index of the physical controller (0–3).
        explicit Controller(unsigned int controllerIndex);
        ~Controller();

        /// @brief Polls the hardware and updates the internal button-state snapshot.
        ///        Must be called once per game loop tick before any query methods.
        void Update();

        /// @brief Returns true if @p button was pressed down this frame (rising edge).
        /// @param button The button to query.
        bool IsDownThisFrame(ControllerButton button) const;

        /// @brief Returns true if @p button was released this frame (falling edge).
        /// @param button The button to query.
        bool IsUpThisFrame(ControllerButton button) const;

        /// @brief Returns true if @p button is currently held down.
        /// @param button The button to query.
        bool IsPressed(ControllerButton button) const;

        /// @brief Returns the right thumbstick's current axis values, normalised to [-1, 1].
        /// @return A 2D vector where x is the horizontal axis and y is the vertical axis.
        glm::vec2 GetRightThumbstick() const;

        /// @brief Returns true if the controller is currently connected to the system.
        bool IsConnected() const;

    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;
    };

} // namespace dae
#endif