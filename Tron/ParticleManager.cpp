#include "ParticleManager.h"
#include "GameTime.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
void Tron::ParticleManager::Init()
{
    m_ExplosionTex = dae::ResourceManager::GetInstance().LoadTexture("Explosion.png");

    if (m_ExplosionTex)
    {
        auto size = m_ExplosionTex->GetSize();
        m_FrameWidth = static_cast<float>(size.x) / 4.0f; // 4 columns
        m_FrameHeight = static_cast<float>(size.y) / 1.0f; // 1 row
    }
}

void Tron::ParticleManager::SpawnExplosion(glm::vec3 position, float frameDuration)
{
    void* rawMemory = m_Allocator.Acquire(sizeof(ParticleData));
    if (!rawMemory) return;

    position.x -= (m_FrameWidth / 2.0f);
    position.y -= (m_FrameHeight / 2.0f);

    ParticleData* newParticle = new (rawMemory) ParticleData(position, glm::vec3{},nullptr, frameDuration, 4);
    m_ActiveParticles.push_back(newParticle);
}
void Tron::ParticleManager::SpawnAttachedExplosion(dae::GameObject* target, glm::vec3 hitWorldPos, float frameDuration)
{
    if (!target)
    {
        SpawnExplosion(hitWorldPos, frameDuration);
        return;
    }

    void* rawMemory = m_Allocator.Acquire(sizeof(ParticleData));
    if (!rawMemory) return;

    // Center the explosion sprite on the hit coordinate
    hitWorldPos.x -= (m_FrameWidth / 2.0f);
    hitWorldPos.y -= (m_FrameHeight / 2.0f);

    // Calculate the mathematical distance between the tank's origin and the bullet impact
    glm::vec3 targetPos = target->GetTransform()->GetWorldPosition();
    glm::vec3 localOffset = hitWorldPos - targetPos;

    ParticleData* newParticle = new (rawMemory) ParticleData(hitWorldPos, localOffset, target, frameDuration, 4);
    m_ActiveParticles.push_back(newParticle);
}


void Tron::ParticleManager::Update()
{
    float dt = Time::GetInstance().GetDeltaTime();

    for (int i = static_cast<int>(m_ActiveParticles.size()) - 1; i >= 0; --i)
    {
        ParticleData* p = m_ActiveParticles[i];

        if (p->attachedTarget)
        {
            if (p->attachedTarget->IsMarkedForDestruction())
            {
                p->attachedTarget = nullptr; 
            }
            else
            {
               
                glm::vec3 targetPos = p->attachedTarget->GetTransform()->GetWorldPosition();
                p->worldPosition = targetPos + p->localOffset;
            }
        }

        p->frameTimer += dt;
        if (p->frameTimer >= p->frameDuration)
        {
            p->frameTimer -= p->frameDuration;
            p->currentFrame++;

            if (p->currentFrame >= p->maxFrames)
            {
                p->~ParticleData();
                m_Allocator.Release(p);
                m_ActiveParticles.erase(m_ActiveParticles.begin() + i);
            }
        }
    }
}

void Tron::ParticleManager::Render() const
{
    auto& renderer = dae::Renderer::GetInstance();

    for (const auto* p : m_ActiveParticles)
    {
        float srcX = p->currentFrame * m_FrameWidth;
        float srcY = 0.0f;
        renderer.RenderTexture(*m_ExplosionTex, p->worldPosition.x, p->worldPosition.y, srcX, srcY, m_FrameWidth, m_FrameHeight);
    }
}
void Tron::ParticleManager::ClearAll()
{
    for (auto* p : m_ActiveParticles)
    {
        p->~ParticleData();
        m_Allocator.Release(p);
    }
    m_ActiveParticles.clear();
}