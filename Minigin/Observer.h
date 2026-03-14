#ifndef OBSERVER_H
#define OBSERVER_H
#include "Event.h"

namespace dae
{
	class GameObject;
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(GameObject* obj,const Event& event ) = 0;
	};
}
#endif
