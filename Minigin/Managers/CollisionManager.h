#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include <vector>
#include "Singleton.h"


namespace dae
{

	class ColliderComponent;

	class CollisionManager : public dae::Singleton<CollisionManager>
	{
	public:

		void Update();
		void AddCollider(ColliderComponent* collider);
		void RemoveCollider(ColliderComponent* collider);

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		std::vector<ColliderComponent*> m_Colliders;
	};

}
#endif


