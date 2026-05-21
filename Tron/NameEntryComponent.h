#ifndef NAMEENTRY_COMP_H
#define NAMEENTRY_COMP_H
#include <string>
#include <functional>
#include "BaseComponent.h"  

namespace Tron
{

    class NameEntryComponent final : public dae::BaseComponent
    {
    public:
        static constexpr int   NUM_SLOTS = 3;
        static constexpr int   NUM_SYMBOLS = 28; // A-Z + ' ' + '_'

        explicit NameEntryComponent(dae::GameObject* owner, int playerIndex);
        ~NameEntryComponent() override = default;

        void ScrollUp();   
        void ScrollDown();  
        void MoveLeft();   
        void MoveRight();  
        void Confirm();   

        bool        IsDone()          const { return m_Done; }
        int         GetPlayerIndex()  const { return m_PlayerIndex; }
        std::string GetName()         const;

     
        char  GetSymbol(int slot)     const;
        int   GetActiveSlot()         const { return m_ActiveSlot; }

        void SetOnConfirmed(std::function<void(int playerIndex, const std::string& name)> cb)
        {
            m_OnConfirmed = std::move(cb);
        }

        void Update() override {}
        void Render() const override {}

    private:
        static char IndexToChar(int idx);

        int  m_PlayerIndex{};
        int  m_ActiveSlot{ 0 };
        int  m_SlotIndices[NUM_SLOTS]{ 0, 0, 0 }; 
        bool m_Done{ false };

        std::function<void(int, const std::string&)> m_OnConfirmed;
    };
}

#endif
