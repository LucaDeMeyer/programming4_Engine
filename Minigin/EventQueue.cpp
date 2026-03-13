#include "EventQueue.h"

void dae::EventQueue::AddEvent(std::unique_ptr<dae::Event> newEvent)
{
    m_EventQueue.push(std::move(newEvent));
}

void dae::EventQueue::Process()
{
    while (!m_EventQueue.empty())
    {
        auto& currentEvent = m_EventQueue.front();

        m_Notifier.Notify(nullptr, *currentEvent);

        m_EventQueue.pop();
    }
}