#ifndef OBSERVER_H
#define OBSERVER_H


namespace dae
{
	class GameObject;
	class ColliderComponent;

	struct Event {
		virtual ~Event() = default;
	};

	struct CollisionEvent final : public Event
	{
		ColliderComponent* Collider1;
		ColliderComponent* Collider2;

		CollisionEvent(ColliderComponent* c1, ColliderComponent* c2)
			: Collider1(c1), Collider2(c2) {
		}
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* obj,const Event& event ) = 0;
	};
}
#endif
