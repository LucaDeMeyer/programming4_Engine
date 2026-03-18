#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>

#include "glm/vec2.hpp"

namespace dae
{
    class Controller final
    {
    public:
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
        explicit Controller(unsigned int controllerIndex);
        ~Controller();
        void Update();

        bool IsDownThisFrame(ControllerButton button) const;
        bool IsUpThisFrame(ControllerButton button) const;
        bool IsPressed(ControllerButton button) const;
        glm::vec2 GetRightThumbstick() const;
    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;
    };
}
#endif