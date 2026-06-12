#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "Observer.h"

namespace dae
{
	/// @brief Maintains a list of Observers and broadcasts events to them.
	///
	/// Part of the Observer pattern. Any system that needs to fire events
	/// owns (or references) a Subject and calls Notify() when something
	/// of interest happens. The Subject iterates its observer list and
	/// calls OnNotify() on each one.
	///
	/// Observer lifetime is managed externally; call RemoveObserver()
	/// before an Observer is destroyed to avoid dangling pointer callbacks.
	class Subject
	{
	public:
		~Subject();

		/// @brief Registers an observer to receive future notifications.
		/// @param observer Non-owning pointer to the observer to add.
		void AddObserver(Observer* observer);

		/// @brief Unregisters an observer so it no longer receives notifications.
		/// @param observer Pointer to the observer to remove.
		void RemoveObserver(Observer* observer);

		/// @brief Broadcasts an event to all currently registered observers.
		/// @param obj   The GameObject associated with the event (may be nullptr).
		/// @param event The event to broadcast.
		void Notify(GameObject* obj, const Event& event);

	private:
		std::vector<Observer*> m_Observers; ///< Non-owning list of registered observers.
	};

} // namespace dae
#endif