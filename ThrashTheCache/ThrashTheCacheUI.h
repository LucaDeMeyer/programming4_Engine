#ifndef THRASH_THE_CACHE_UI_H
#define THRASH_THE_CACHE_UI_H
#include "ThrashTheCache.h"
#include "ImguiComponent.h"
namespace dae {
    class ThrashTheCacheUI : public ImguiComponent {
    public:

        explicit ThrashTheCacheUI(dae::GameObject* owner) : ImguiComponent(owner)
        {
        }

        void Update() override{}
      
        void RenderUI() override;

    private:
        TestResult m_Ex1Results;
        TestResult m_Ex2Results;
        TestResult m_Ex2AltResults;

        int m_NumSamples = 0;
    };
}

#endif