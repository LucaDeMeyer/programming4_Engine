#ifndef NAME_ENTRY_COMMAND_H
#define NAME_ENTRY_COMMAND_H
#include "Command.h"
#include "NameEntryComponent.h"

namespace Tron
{
    // Scroll the active letter slot up (A -> B -> C ...)
    class ScrollUpCommand final : public dae::ActorCommand
    {
    public:
        ScrollUpCommand(dae::GameObject* obj, NameEntryComponent* entry)
            : dae::ActorCommand(obj), m_Entry(entry) {}

        void Execute() override
        {
            if (m_Entry) m_Entry->ScrollUp();
        }
    private:
        NameEntryComponent* m_Entry;
    };

    // Scroll the active letter slot down
    class ScrollDownCommand final : public dae::ActorCommand
    {
    public:
        ScrollDownCommand(dae::GameObject* obj, NameEntryComponent* entry)
            : dae::ActorCommand(obj), m_Entry(entry) {}

        void Execute() override
        {
            if (m_Entry) m_Entry->ScrollDown();
        }
    private:
        NameEntryComponent* m_Entry;
    };

    // Move cursor one slot to the left
    class SlotLeftCommand final : public dae::ActorCommand
    {
    public:
        SlotLeftCommand(dae::GameObject* obj, NameEntryComponent* entry)
            : dae::ActorCommand(obj), m_Entry(entry) {}

        void Execute() override
        {
            if (m_Entry) m_Entry->MoveLeft();
        }
    private:
        NameEntryComponent* m_Entry;
    };

    // Move cursor one slot to the right
    class SlotRightCommand final : public dae::ActorCommand
    {
    public:
        SlotRightCommand(dae::GameObject* obj, NameEntryComponent* entry)
            : dae::ActorCommand(obj), m_Entry(entry) {}

        void Execute() override
        {
            if (m_Entry) m_Entry->MoveRight();
        }
    private:
        NameEntryComponent* m_Entry;
    };

    // Confirm / lock in the name
    class ConfirmNameCommand final : public dae::ActorCommand
    {
    public:
        ConfirmNameCommand(dae::GameObject* obj, NameEntryComponent* entry)
            : dae::ActorCommand(obj), m_Entry(entry) {}

        void Execute() override
        {
            if (m_Entry) m_Entry->Confirm();
        }
    private:
        NameEntryComponent* m_Entry;
    };

    class ConfirmCommand final : public dae::ActorCommand
    {
    public:
        ConfirmCommand(dae::GameObject* obj, std::function<void()> callback) : ActorCommand(obj), m_Callback(std::move(callback)) {}

        void Execute() override
        {
            if (m_Callback)
                m_Callback();
        }
    private:
        std::function<void()> m_Callback;
    };
}

#endif