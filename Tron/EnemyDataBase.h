#ifndef ENEMY_DATABASE_H
#define ENEMY_DATABASE_H
#include <map>
#include <string>
#include <memory>

#include "Singleton.h"

namespace Tron
{
    enum class AIType { Tank, Recogniser };

    struct EnemyProfile
    {
        float speed;
        int maxLives;
        int pointValue;
        float visionRadius;
        std::string textureName;
       
    };


    class EnemyDatabase : public dae::Singleton<EnemyDatabase>
    {
    public:

        void Init()
        {
            m_Profiles[AIType::Tank] = { 50.f, 2, 100,600, "BlueTank_SpriteSheet.png" };
            m_Profiles[AIType::Recogniser] = { 100.f, 1, 150, 300,"Recogniser.png" };
        }

        const EnemyProfile* GetProfile(AIType type) const
        {
            auto it = m_Profiles.find(type);
            if (it != m_Profiles.end()) {
                return &(it->second);
            }
            return nullptr;
        }

    private:
        friend class dae::Singleton<EnemyDatabase>;
        EnemyDatabase() = default;
        ~EnemyDatabase() override = default;

        std::map<AIType, EnemyProfile> m_Profiles;
    };
}

#endif
