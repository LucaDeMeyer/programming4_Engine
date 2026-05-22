#include "NameEntryDisplay.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"

namespace Tron
{
    static constexpr int NUM_SYMBOLS = NameEntryComponent::NUM_SYMBOLS;
    static constexpr char s_Syms[NUM_SYMBOLS] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
        ' ','_'
    };

    char NameEntryDisplay::PeekSymbol(int slot, int offset) const
    {
        char centre = m_Entry->GetSymbol(slot);
        int idx = 0;
        for (int i = 0; i < NUM_SYMBOLS; ++i) {
            if (s_Syms[i] == centre) { idx = i; break; }
        }
        return s_Syms[(idx + offset + NUM_SYMBOLS) % NUM_SYMBOLS];
    }

    void NameEntryDisplay::Init()
    {
        if (m_Initialized) return;
        m_Initialized = true;

        // Title label ("PLAYER 1" or "PLAYER 2")
        {
            auto labelObj = std::make_unique<dae::GameObject>();
            labelObj->GetTransform()->SetLocalPosition({ m_Origin.x - 40.f, m_Origin.y - 80.f, m_Origin.z });
            labelObj->AddComponent<dae::TextComponent>()
                ->SetFont("TRON.TTF", 20)
                ->SetText(m_Label)
                ->SetColor(0, 200, 255, 255);
            m_Scene->Add(std::move(labelObj));
        }

        for (int s = 0; s < NameEntryComponent::NUM_SLOTS; ++s)
        {
            float x = m_Origin.x + s * SLOT_SPACING;

            // --- "above" letter (slot + 1), small, dim ---
            {
                auto go = std::make_unique<dae::GameObject>();
                go->GetTransform()->SetLocalPosition({ x, m_Origin.y - 40.f, m_Origin.z });
                m_Slots[s].above = go->AddComponent<dae::TextComponent>();
                m_Slots[s].above->SetFont("TRON.TTF", static_cast<int>(FONT_SMALL));
                m_Slots[s].above->SetColor(100, 100, 100, 255);
                m_Scene->Add(std::move(go));
            }

            // --- current letter, BIG and lit ---
            {
                auto go = std::make_unique<dae::GameObject>();
                go->GetTransform()->SetLocalPosition({ x, m_Origin.y, m_Origin.z });
                m_Slots[s].current = go->AddComponent<dae::TextComponent>();
                m_Slots[s].current->SetFont("TRON.TTF", static_cast<int>(FONT_BIG));
                m_Slots[s].current->SetColor(255, 255, 255, 255);
                m_Scene->Add(std::move(go));
            }

            // --- "below" letter (slot - 1), small, dim ---
            {
                auto go = std::make_unique<dae::GameObject>();
                go->GetTransform()->SetLocalPosition({ x, m_Origin.y + 50.f, m_Origin.z });
                m_Slots[s].below = go->AddComponent<dae::TextComponent>();
                m_Slots[s].below->SetFont("TRON.TTF", static_cast<int>(FONT_SMALL));
                m_Slots[s].below->SetColor(100, 100, 100, 255);
                m_Scene->Add(std::move(go));
            }

            // --- cursor underline "^" indicator ---
            {
                auto go = std::make_unique<dae::GameObject>();
                go->GetTransform()->SetLocalPosition({ x + 6.f, m_Origin.y + 90.f, m_Origin.z });
                m_Slots[s].cursor = go->AddComponent<dae::TextComponent>();
                m_Slots[s].cursor->SetFont("TRON.TTF", 20);
                m_Slots[s].cursor->SetText("^");
                m_Slots[s].cursor->SetColor(0, 200, 255, 255);
                m_Scene->Add(std::move(go));
            }
        }

        // "DONE" flash text that appears once confirmed
        {
            auto go = std::make_unique<dae::GameObject>();
            go->GetTransform()->SetLocalPosition({ m_Origin.x + 10.f, m_Origin.y + 130.f, m_Origin.z });
            m_DoneText = go->AddComponent<dae::TextComponent>();
            m_DoneText->SetFont("TRON.TTF", 18);
            m_DoneText->SetText("");
            m_DoneText->SetColor(0, 255, 100, 255);
            m_Scene->Add(std::move(go));
        }
    }

    void NameEntryDisplay::Update()
    {
        if (!m_Initialized) return;

        int activeSlot = m_Entry->GetActiveSlot();

        for (int s = 0; s < NameEntryComponent::NUM_SLOTS; ++s)
        {
            bool isActive = (s == activeSlot) && !m_Entry->IsDone();

           
            std::string cur(1, m_Entry->GetSymbol(s));
            std::string above(1, PeekSymbol(s, +1));
            std::string below(1, PeekSymbol(s, -1));

            if (m_Slots[s].current) m_Slots[s].current->SetText(cur);
            if (m_Slots[s].above)   m_Slots[s].above->SetText(above);
            if (m_Slots[s].below)   m_Slots[s].below->SetText(below);

            
            if (m_Slots[s].current)
            {
                if (m_Entry->IsDone())
                    m_Slots[s].current->SetColor(0, 255, 100, 255); 
                else if (isActive)
                    m_Slots[s].current->SetColor(0, 200, 255, 255); 
                else
                    m_Slots[s].current->SetColor(255, 255, 255, 255); 
            }

         
            if (m_Slots[s].cursor)
                m_Slots[s].cursor->SetText(isActive ? "^" : "");
        }

        if (m_DoneText)
            m_DoneText->SetText(m_Entry->IsDone() ? "READY!" : "");
    }
}