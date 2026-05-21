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
    // Renders one player's 3-letter arcade spinner.
    // Add this to the same GameObject that has NameEntryComponent, then call
    // Init() after the scene is set up so it can create its child text objects.
    //
    // Visual layout (example for Player 1, centered at x=300):
    //
    //   ENTER YOUR NAME
    //       ^ ^ ^          <- "above" letters (slot+1)
    //     [ A   A   A ]   <- active row (big, lit)
    //       v v v          <- "below" letters (slot-1)
    //     _________
    //      cursor ^
    //
    // Uses your existing TextComponent for each glyph row.

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

        // Call once after adding to scene to create the child text GameObjects.
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

        // One text object per slot per row (above / current / below)
        // Plus one cursor indicator per slot
        struct SlotUI
        {
            dae::TextComponent* above{};
            dae::TextComponent* current{};
            dae::TextComponent* below{};
            dae::TextComponent* cursor{};
        };
        std::array<SlotUI, NameEntryComponent::NUM_SLOTS> m_Slots{};
        dae::TextComponent* m_DoneText{};

        // Helper: peek at symbol N steps away from slot's current index
        char PeekSymbol(int slot, int offset) const;
    };
}
