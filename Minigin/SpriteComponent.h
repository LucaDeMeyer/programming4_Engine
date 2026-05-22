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
        explicit SpriteComponent(GameObject* owner, const std::string& filename, int cols, int rows,int totalFrames);

        void Update() override;
        void Render() const override;

        void SetFrame(int frame) { m_CurrentFrame = frame; }
        int GetFrame() const { return m_CurrentFrame; }

        void SetVisible(bool visible) { m_IsVisible = visible; }
        bool IsVisible() const { return m_IsVisible; }

        void Play(float duration, bool loop);
      
        bool IsPlaying() { return m_IsPlaying; }
    private:
        std::shared_ptr<Texture2D> m_Texture;
        int m_Cols;
        int m_Rows;
        int m_CurrentFrame{ 0 };

        float m_FrameWidth{ 0 };
        float m_FrameHeight{ 0 };

        bool m_IsVisible{ true };

        float m_AnimationTimer{ 0.0f };
        float m_FrameDuration{ 0.0f };
        int m_TotalFrames;
        bool m_Loop{ false };
        bool m_IsPlaying{ false };
    };
}

#endif