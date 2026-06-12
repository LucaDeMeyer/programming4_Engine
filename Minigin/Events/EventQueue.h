#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include <memory>
#include <queue>

#include "Observer.h"
#include "Singleton.h"
#include "Subject.h"

namespace dae
{
	/// @brief Global deferred event bus, implemented as a Singleton.
	///
	/// Producers push Events into the queue via AddEvent(). Once per frame,
	/// Process() dequeues every pending event and forwards each one to the
	/// internal Subject, which in turn notifies all registered Observers.
	///
	/// This decouples event producers from consumers and ensures that all
	/// event handling happens at a predictable point in the game loop.
	///
	/// @note Observers should register themselves via GetNotifier()->AddObserver().
	class EventQueue : public Singleton<EventQueue>
	{
	public:
		/// @brief Pushes a new event onto the back of the queue.
		/// @param newEvent The event to enqueue (moved in).
		void AddEvent(Event&& newEvent);

		/// @brief Drains the queue and notifies observers for each pending event.
		///        Call once per frame, typically during the engine's Update pass.
		void Process();

		/// @brief Returns the internal Subject used to register/remove observers.
		/// @return Pointer to the Subject that broadcasts processed events.
		Subject* GetNotifier() { return &m_Notifier; }

	private:
		friend class Singleton<EventQueue>;
		EventQueue() = default;
		~EventQueue() = default;

		std::queue<Event> m_EventQueue; ///< Pending events awaiting processing.
		Subject           m_Notifier;   ///< Broadcasts each processed event to observers.
	};

} // namespace dae

#endif