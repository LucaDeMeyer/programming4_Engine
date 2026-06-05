#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include <memory>
#include <queue>

#include "Observer.h"
#include "Singleton.h"
#include "Subject.h"

namespace dae
{
	class EventQueue : public Singleton<EventQueue>
	{
	public:
		void AddEvent(Event&& newEvent);

		void Process();

		Subject* GetNotifier() { return &m_Notifier; }

	private:
		friend class Singleton<EventQueue>;
		EventQueue() = default;
		~EventQueue() = default;

		std::queue<Event> m_EventQueue;
		Subject m_Notifier;
	};
}


#endif
