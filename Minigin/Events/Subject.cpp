#include "Subject.h"


dae::Subject::~Subject()
{
	for (auto& observer : m_Observers)
	{
		RemoveObserver(observer);
	}
}
void dae::Subject::AddObserver(Observer* observer)
{
	if (std::find(m_Observers.begin(),m_Observers.end(),observer) == m_Observers.end())
	{
		m_Observers.emplace_back(observer);
	}
}

void dae::Subject::RemoveObserver(Observer* observer)
{
	std::erase(m_Observers,observer);
}

void dae::Subject::Notify(GameObject* obj, const Event& event)
{
	for (auto* observer : m_Observers)
	{
		observer->OnNotify(obj, event);
	}
}
