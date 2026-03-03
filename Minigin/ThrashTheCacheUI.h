#ifndef THRASH_THE_CACHE_UI_H
#define THRASH_THE_CACHE_UI_H
#include "ThrashTheCache.h"

namespace dae {
    class ThrashTheCacheUI {
    public:
        void Render();

    private:
        TestResult m_Ex1Results;
        TestResult m_Ex2Results;
        TestResult m_Ex2AltResults;

        int m_NumSamples;
    };
}

#endif