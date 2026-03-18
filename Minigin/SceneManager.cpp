#include "SceneManager.h"

#include <stdexcept>

#include "Scene.h"

void dae::SceneManager::Update()
{
	if (!m_scenes.empty())
	{
		m_scenes[m_ActiveSceneIndex]->Update();
		m_scenes[m_ActiveSceneIndex]->LateUpdate();
	}
}

void dae::SceneManager::ClearScenes()
{
	m_scenes.clear();
}

void dae::SceneManager::Render()
{
	if (!m_scenes.empty())
	{
		m_scenes[m_ActiveSceneIndex]->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

void dae::SceneManager::RenderUI()
{
	if (!m_scenes.empty())
	{
		m_scenes[m_ActiveSceneIndex]->RenderUI();
	}
}

dae::Scene& dae::SceneManager::GetActiveScene()
{
	if (m_scenes.empty())
	{
		throw std::runtime_error("No scenes have been created yet!");
	}
	return *m_scenes[m_ActiveSceneIndex];
}
void dae::SceneManager::SetActiveScene(size_t index)
{
	if (index < m_scenes.size())
	{
		m_ActiveSceneIndex = index;
	}
}