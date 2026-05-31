#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H
#include "FactionComponent.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Memory/PoolAllocator.h"
#include "Singleton.h"
#include "Texture2D.h"
#include "glm/vec4.hpp"

namespace dae
{
	class GameObject;
}

namespace Tron
{
	struct BulletData
	{
        glm::vec3 position;
        int maxBounces;
        glm::vec2 velocity;
        dae::GameObject* shooter;
        Team team;

        BulletData(glm::vec3 pos, glm::vec2 vel, Team t, dae::GameObject* s)
            : position(pos), maxBounces(5), velocity(vel), shooter(s), team(t) {
        }
    };

    class BulletManager final :  public dae::Singleton<BulletManager>
    {
    public:

        void Init();

        void FireBullet(glm::vec3 startPos, glm::vec2 velocity, Team team, dae::GameObject* shooter);

        void Update();
        void Render() const;
        void ClearAll();
    private:
        friend class dae::Singleton<BulletManager>;
        BulletManager() = default;

        bool IsOverlapping(const glm::vec4& box1, const glm::vec4& box2);

        dae::PoolAllocator<sizeof(BulletData), 50> m_Allocator;
        std::vector<BulletData*> m_ActiveBullets;

        std::shared_ptr<dae::Texture2D> m_PlayerBulletTex;
        std::shared_ptr<dae::Texture2D> m_EnemyBulletTex;
    };
	
}
#endif
