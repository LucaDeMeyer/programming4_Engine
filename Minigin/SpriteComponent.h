#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H
#include "BaseComponent.h"
#include "Texture2D.h"
#include <memory>

namespace dae
{
    class SpriteComponent final : public BaseComponent
    {
    public:
        explicit SpriteComponent(GameObject* owner, const std::string& filename, int cols, int rows);
    

        void Update() override {}
        void Render() const override;

        void SetFrame(int frame) { m_CurrentFrame = frame; }
        int GetFrame() const { return m_CurrentFrame; }
    private:
        std::shared_ptr<Texture2D> m_Texture;
        int m_Cols;
        int m_Rows;
        int m_CurrentFrame{ 0 };

        float m_FrameWidth{ 0 };
        float m_FrameHeight{ 0 };
    };
}

#endif