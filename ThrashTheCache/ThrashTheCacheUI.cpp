#include "ThrashTheCacheUI.h"
#include <imgui.h>

void dae::ThrashTheCacheUI::RenderUI() {
    ImGui::Begin("Cache Benchmark exercise");

    ImGui::InputInt("Samples", &m_NumSamples);
    m_NumSamples = std::max(3, m_NumSamples);

    if (ImGui::Button("Run Int Benchmark")) {
        m_Ex1Results = ThrashTheCache::RunExercise1(m_NumSamples);
    }
    if (!m_Ex1Results.results.empty()) {
        ImGui::PlotLines("Ints", m_Ex1Results.results.data(), (int)m_Ex1Results.results.size(), 0, "ms", 0.0f, FLT_MAX, ImVec2(0, 80));
    }
    if (ImGui::Button("Run GameObject3D Benchmark")) {
        m_Ex2Results = ThrashTheCache::RunExercise2(m_NumSamples);
    }
    if (!m_Ex2Results.results.empty()) {
        ImGui::PlotLines("GameObject3D", m_Ex2Results.results.data(), (int)m_Ex2Results.results.size(), 0, "ms", 0.0f, FLT_MAX, ImVec2(0, 80));
    }
    if (ImGui::Button("Run GameObject3DALT Benchmark")) {
        m_Ex2AltResults = ThrashTheCache::RunExercise2Alt(m_NumSamples);
    }
    if (!m_Ex2AltResults.results.empty()) {
        ImGui::PlotLines("GameObject3DALT", m_Ex2AltResults.results.data(), (int)m_Ex2AltResults.results.size(), 0, "ms", 0.0f, FLT_MAX, ImVec2(0, 80));
    }
    ImGui::End();
}