#ifndef OBSERVER_H
#define OBSERVER_H
#include "Event.h"

namespace dae
{
	class GameObject;

	/// @brief Interface for objects that want to receive event notifications.
	///
	/// Part of the Observer pattern. Register an Observer with a Subject (or
	/// via EventQueue::GetNotifier()) to receive OnNotify() callbacks whenever
	/// a matching event is broadcast.
	///
	/// Concrete subclasses should check the Event::ID to determine whether they
	/// care about a given notification, then cast Event::pArgs to the appropriate
	/// EventARGS subtype if additional data is needed.
	class Observer
	{
	public:
		virtual ~Observer() = default;

		/// @brief Called by a Subject when an event is broadcast.
		/// @param obj   The GameObject associated with the event, if any. May be nullptr.
		/// @param event The event that was fired, including its ID and optional payload.
		// TODO: obj is currently unused in all implementations — consider removing it.
		virtual void OnNotify(GameObject* obj, const Event& event) = 0;
	};

} // namespace dae
#endif