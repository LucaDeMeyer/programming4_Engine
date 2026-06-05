#pragma once
#include "BaseComponent.h"
#include "NameEntryComponent.h"
#include "GameObject.h"
#include "TextComponent.h"   // your engine's text component
#include <array>
#include <string>

#include "Scene.h"
#include "glm/vec3.hpp"

namespace Tron
{
    class NameEntryDisplay final : public dae::BaseComponent
    {
    public:
        NameEntryDisplay(dae::GameObject* owner,
            NameEntryComponent* entry,
            dae::Scene* scene,
            glm::vec3 origin,
            const std::string& label)
            : dae::BaseComponent(owner)
            , m_Entry(entry)
            , m_Scene(scene)
            , m_Origin(origin)
            , m_Label(label)
        {
        }

        ~NameEntryDisplay() override = default;

      
        void Init();

        void Update() override;
        void Render() const override {}

    private:
        static constexpr float SLOT_SPACING = 60.f;
        static constexpr float FONT_BIG = 40.f;
        static constexpr float FONT_SMALL = 22.f;

        NameEntryComponent* m_Entry{};
        dae::Scene* m_Scene{};
        glm::vec3           m_Origin{};
        std::string         m_Label{};

        bool m_Initialized{ false };

      
        struct SlotUI
        {
            dae::TextComponent* above{};
            dae::TextComponent* current{};
            dae::TextComponent* below{};
            dae::TextComponent* cursor{};
        };
        std::array<SlotUI, NameEntryComponent::NUM_SLOTS> m_Slots{};
        dae::TextComponent* m_DoneText{};

      
        char PeekSymbol(int slot, int offset) const;
    };
}
