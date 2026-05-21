#include "NameEntryComponent.h"
#include "GameObject.h"

namespace Tron
{
    static constexpr char s_Symbols[NameEntryComponent::NUM_SYMBOLS] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
        ' ','_'
    };

    NameEntryComponent::NameEntryComponent(dae::GameObject* owner, int playerIndex)
        : dae::BaseComponent(owner)
        , m_PlayerIndex(playerIndex)
    {
    }

    char NameEntryComponent::IndexToChar(int idx)
    {
        return s_Symbols[idx % NUM_SYMBOLS];
    }

    char NameEntryComponent::GetSymbol(int slot) const
    {
        return IndexToChar(m_SlotIndices[slot]);
    }

    std::string NameEntryComponent::GetName() const
    {
        std::string name;
        name.reserve(NUM_SLOTS);
        for (int i = 0; i < NUM_SLOTS; ++i)
            name += IndexToChar(m_SlotIndices[i]);

        // Trim trailing spaces/underscores so "AAA___" becomes "AAA"
        while (!name.empty() && (name.back() == ' ' || name.back() == '_'))
            name.pop_back();

        return name.empty() ? "---" : name;
    }

    void NameEntryComponent::ScrollUp()
    {
        if (m_Done) return;
        m_SlotIndices[m_ActiveSlot] = (m_SlotIndices[m_ActiveSlot] + 1) % NUM_SYMBOLS;
    }

    void NameEntryComponent::ScrollDown()
    {
        if (m_Done) return;
        m_SlotIndices[m_ActiveSlot] = (m_SlotIndices[m_ActiveSlot] - 1 + NUM_SYMBOLS) % NUM_SYMBOLS;
    }

    void NameEntryComponent::MoveLeft()
    {
        if (m_Done) return;
        if (m_ActiveSlot > 0) --m_ActiveSlot;
    }

    void NameEntryComponent::MoveRight()
    {
        if (m_Done) return;
        if (m_ActiveSlot < NUM_SLOTS - 1) ++m_ActiveSlot;
    }

    void NameEntryComponent::Confirm()
    {
        if (m_Done) return;
        m_Done = true;
        if (m_OnConfirmed)
            m_OnConfirmed(m_PlayerIndex, GetName());
    }
}