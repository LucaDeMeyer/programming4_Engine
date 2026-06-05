#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
#include "glm/vec3.hpp"
#include "Memory/PoolAllocator.h"
#include "Singleton.h"
#include "Texture2D.h"
#include "GameObject.h" 
#include <vector>
#include <memory>
namespace dae
{
	class GameObject;
}

namespace Tron
{
    struct ParticleData
    {
        glm::vec3 worldPosition;
        glm::vec3 localOffset;          
        dae::GameObject* attachedTarget;  

        float frameTimer;
        float frameDuration;
        int currentFrame;
        int maxFrames;

        ParticleData(glm::vec3 wPos, glm::vec3 offset, dae::GameObject* target, float duration, int frames)
            : worldPosition(wPos), localOffset(offset), attachedTarget(target),
            frameTimer(0.0f), frameDuration(duration), currentFrame(0), maxFrames(frames) {
        }
    };

    class ParticleManager final : public dae::Singleton<ParticleManager>
    {
    public:
        void Init();
        void Update();
        void Render() const;
        void ClearAll();

        void SpawnExplosion(glm::vec3 position, float frameDuration = 0.1f);

        void SpawnAttachedExplosion(dae::GameObject* target, glm::vec3 hitWorldPos, float frameDuration = 0.1f);

    private:
        friend class dae::Singleton<ParticleManager>;
        ParticleManager() = default;

        dae::PoolAllocator<sizeof(ParticleData), 100> m_Allocator;
        std::vector<ParticleData*> m_ActiveParticles;

        std::shared_ptr<dae::Texture2D> m_ExplosionTex;
        float m_FrameWidth{ 0 };
        float m_FrameHeight{ 0 };
    };
}
#endif