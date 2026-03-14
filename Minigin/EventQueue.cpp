#include "EventQueue.h"

void dae::EventQueue::AddEvent(Event&& newEvent)
{
    m_EventQueue.push(std::move(newEvent));
}

void dae::EventQueue::Process()
{
    while (!m_EventQueue.empty())
    {
        dae::Event currentEvent = std::move(m_EventQueue.front());

        m_EventQueue.pop();

        m_Notifier.Notify(nullptr, currentEvent);

      
    }
}