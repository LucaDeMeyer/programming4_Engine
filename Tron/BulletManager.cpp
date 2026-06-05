#include "BulletManager.h"

#include "ColliderComponents.h"
#include "EventQueue.h"
#include "GameManager.h"
#include "GameTime.h"
#include "LevelManager.h"
#include "Components/LivesComponent.h"
#include "Minigin.h"
#include "ParticleManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "glm/vec4.hpp"
#include "Components/PlayerComponent.h"
#include "Memory/MemoryOverrides.h"
void Tron::BulletManager::Init()
{
    auto& rm = dae::ResourceManager::GetInstance();
    m_PlayerBulletTex = rm.LoadTexture("Tank_Bullet.png");
    m_EnemyBulletTex = rm.LoadTexture("Bullet_Enemy.png");
}

void Tron::BulletManager::FireBullet(glm::vec3 startPos, glm::vec2 velocity, Team team, dae::GameObject* shooter)
{
    startPos.x += 12.0f;
    startPos.y += 12.0f;

    void* rawMemory = m_Allocator.Acquire(sizeof(BulletData));
    if (!rawMemory) return;

    BulletData* newBullet = new (rawMemory) BulletData(startPos, velocity, team, shooter);

    m_ActiveBullets.push_back(newBullet);
}

void Tron::BulletManager::Update()
{
    float dt = Time::GetInstance().GetDeltaTime();
    auto& levelManager = LevelManager::GetInstance();
    auto& gameManager = GameManager::GetInstance();

    for (int i = static_cast<int>(m_ActiveBullets.size()) - 1; i >= 0; --i)
    {
        BulletData* bullet = m_ActiveBullets[i];

        bullet->position.x += bullet->velocity.x * dt;
        bullet->position.y += bullet->velocity.y * dt;

        glm::vec4 bulletBox = { bullet->position.x, bullet->position.y, 8.0f, 8.0f };
        bool bulletDestroyed = false;

        glm::vec3 checkPos = { bullet->position.x + 4.0f, bullet->position.y + 4.0f, 0 };

        if (levelManager.IsWallAt(checkPos))
        {
            if (bullet->maxBounces > 0)
            {
                float tileSize = 32.0f;
                float offsetX = levelManager.GetOffsetX();
                float offsetY = levelManager.GetOffsetY();

                float wallX = std::floor((checkPos.x - offsetX) / tileSize) * tileSize + offsetX;
                float wallY = std::floor((checkPos.y - offsetY) / tileSize) * tileSize + offsetY;
                glm::vec4 wallBox = { wallX, wallY, tileSize, tileSize };

                float overlapX = std::min(bulletBox.x + bulletBox.z, wallBox.x + wallBox.z) - std::max(bulletBox.x, wallBox.x);
                float overlapY = std::min(bulletBox.y + bulletBox.w, wallBox.y + wallBox.w) - std::max(bulletBox.y, wallBox.y);

                if (overlapX < overlapY)
                {
                    bullet->velocity.x *= -1;

                    if (bullet->velocity.x > 0)
                        bullet->position.x += overlapX; 
                    else
                        bullet->position.x -= overlapX;
                }
                else
                {
                    bullet->velocity.y *= -1;
                    if (bullet->velocity.y > 0)
                        bullet->position.y += overlapY; 
                    else
                        bullet->position.y -= overlapY;
                }

                bullet->maxBounces -= 1;
            }
            else
            {

                bulletDestroyed = true;
            }
        }
      
        if (!bulletDestroyed)
        {
            for (auto* entity : gameManager.GetEntities())
            {
                auto* collider = entity->GetComponent<dae::BoxColliderComponent>();
                auto* faction = entity->GetComponent<FactionComponent>();

                if (collider && faction && faction->GetTeam() != bullet->team)
                {
                    if (IsOverlapping(bulletBox, collider->GetWorldBox()))
                    {

                        auto* playerComp = entity->GetComponent<PlayerComponent>();
                        if (playerComp && playerComp->IsInvulnerable())
                        {
                            continue; 
                        }

                        auto* soundArgs = new (dae::Minigin::GetFrameAllocator()) dae::SoundARGS(dae::Utils::make_sdbm_hash("Bullet_Explosion"), .5f, dae::AudioType::FX);
                        dae::EventQueue::GetInstance().AddEvent(dae::Event(dae::Utils::make_sdbm_hash("ENGINE_PLAY_AUDIO"), soundArgs));

                        entity->GetComponent<LivesComponent>()->DoDamage(1,bullet->shooter);
                        ParticleManager::GetInstance().SpawnAttachedExplosion(entity, bullet->position, 0.05f);
                        bulletDestroyed = true;
                        break; 
                    }
                }
            }
        }

        if (bulletDestroyed)
        {
            bullet->~BulletData();
            m_Allocator.Release(bullet);
            m_ActiveBullets.erase(m_ActiveBullets.begin() + i);
        }
    }
}

void Tron::BulletManager::Render() const
{
    auto& renderer = dae::Renderer::GetInstance();

    for (const auto* bullet : m_ActiveBullets)
    {
        if (bullet->team == Team::Player1 || bullet->team == Team::Player2)
        {
            renderer.RenderTexture(*m_PlayerBulletTex, bullet->position.x, bullet->position.y);
        }
        else
        {
            renderer.RenderTexture(*m_EnemyBulletTex, bullet->position.x, bullet->position.y);
        }
    }
}

bool Tron::BulletManager::IsOverlapping(const glm::vec4& box1, const glm::vec4& box2)
{
    return (box1.x < box2.x + box2.z &&
        box1.x + box1.z > box2.x &&
        box1.y < box2.y + box2.w &&
        box1.y + box1.w > box2.y);
}

void Tron::BulletManager::ClearAll()
{
    for (auto* bullet : m_ActiveBullets)
    {
        bullet->~BulletData();
        m_Allocator.Release(bullet);
    }
    m_ActiveBullets.clear();
}