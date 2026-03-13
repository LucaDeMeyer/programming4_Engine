#ifndef OBSERVER_H
#define OBSERVER_H


namespace dae
{
	class GameObject;
	struct Event {
		virtual ~Event() = default;
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* obj,const Event& event ) = 0;
	};
}
#endif
