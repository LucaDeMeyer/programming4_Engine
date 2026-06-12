#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include <vector>
#include "Singleton.h"

namespace dae
{
	class ColliderComponent;

	/// @brief Singleton that owns the collision detection pass each frame.
	///
	/// All active ColliderComponents register themselves here on creation and
	/// deregister on destruction. During Update(), the manager performs an
	/// O(n²) overlap check between every pair of registered colliders and fires
	/// a collision Event (with CollisionARGS) through the EventQueue for each
	/// intersecting pair.
	class CollisionManager : public dae::Singleton<CollisionManager>
	{
	public:
		/// @brief Tests all registered collider pairs and fires collision events.
		///        Call once per frame during the engine's Update pass.
		void Update();

		/// @brief Registers a collider so it participates in collision checks.
		/// @param collider Non-owning pointer to the collider to add.
		void AddCollider(ColliderComponent* collider);

		/// @brief Removes a collider from the active set.
		///        Must be called before the collider is destroyed.
		/// @param collider Pointer to the collider to remove.
		void RemoveCollider(ColliderComponent* collider);

		void Clear() { m_Colliders.clear(); }

	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;

		std::vector<ColliderComponent*> m_Colliders; ///< Non-owning list of active colliders.
	};

} // namespace dae
#endif