#include "SpriteComponent.h"
#include "Renderer.h"
#include "SDL3/SDL_rect.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include"TransformComponent.h"

dae::SpriteComponent::SpriteComponent(GameObject* owner, const std::string& filename, int cols, int rows,int totalFrame)
	:BaseComponent(owner),m_Cols(cols),m_Rows(rows),m_TotalFrames(totalFrame)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
    if (m_Texture)
    {
        auto size = m_Texture->GetSize(); 
        m_FrameWidth = static_cast<float>(size.x) / m_Cols;
        m_FrameHeight = static_cast<float>(size.y) / m_Rows;
    }
}

void dae::SpriteComponent::Render() const
{
    if (!m_Texture || !m_IsVisible) return;

    int col = m_CurrentFrame % m_Cols;
    int row = m_CurrentFrame / m_Cols;

    float srcX = col * m_FrameWidth;
    float srcY = row * m_FrameHeight;

    auto pos = GetOwner()->GetTransform()->GetWorldPosition();

    Renderer::GetInstance().RenderTexture(*m_Texture,pos.x, pos.y,srcX, srcY, m_FrameWidth, m_FrameHeight);
}

void dae::SpriteComponent::Update()
{
    if (!m_IsPlaying) return;

    m_AnimationTimer += Time::GetInstance().GetDeltaTime();
    if (m_AnimationTimer >= m_FrameDuration)
    {
        m_AnimationTimer -= m_FrameDuration;
        m_CurrentFrame++;

        if (m_CurrentFrame >= m_TotalFrames)
        {
            if (m_Loop) m_CurrentFrame = 0;
            else {
                m_IsPlaying = false;
                m_IsVisible = false; 
            }
        }
    }
}

void dae::SpriteComponent::Play(float duration, bool loop)
{
    m_FrameDuration = duration / m_TotalFrames;
    m_Loop = loop;
    m_IsPlaying = true;
    m_AnimationTimer = 0.0f;
    m_CurrentFrame = 0;
    m_IsVisible = true;
}
