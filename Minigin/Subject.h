#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>

#include "Observer.h"

namespace dae
{
	class Subject
	{
		public:

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

		void Notify(GameObject* obj, const Event& event);

	private:
		std::vector<Observer*> m_Observers;
	};
}

#endif
